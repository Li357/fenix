#include "ethernet.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "gpio.h"
#include "nvic.h"
#include "rcc.h"
#include "syscfg.h"

static __attribute__((aligned(4))) uint8_t TXB[ETH_TX_BUFFER_NUM][ETH_TX_BUFFER_SIZE];
static __attribute__((aligned(4))) uint8_t RXB[ETH_RX_BUFFER_NUM][ETH_RX_BUFFER_SIZE];
static __attribute__((aligned(4))) eth_des_t TXDL[ETH_TX_BUFFER_NUM];
static __attribute__((aligned(4))) eth_des_t RXDL[ETH_RX_BUFFER_NUM];
static eth_des_t *currTXD;
static eth_des_t *currRXD;

static eth_receive_frame_cb_t _eth_receive_frame_cb;

volatile int _eth_received_frame;

void eth_phy_write(uint8_t address, uint8_t reg, uint16_t value) {
  // Note we implicitly have clock range to be HCLK / 42 <= 2.5MHz
  ETH->MACMIIDR = value;
  ETH->MACMIIAR |=
      (address << ETH_MACMIIAR_PASHIFT) | (reg << ETH_MACMIIAR_MRSHIFT) | ETH_MACMIIAR_MW;

#ifndef DEBUG
  // Wait until write finished
  while (ETH->MACMIIAR & ETH_MACMIIAR_MB) {}
#endif
}

uint16_t eth_phy_read(uint8_t address, uint8_t reg) {
  ETH->MACMIIAR |= (address << ETH_MACMIIAR_PASHIFT) | (reg << ETH_MACMIIAR_MRSHIFT);
#ifndef DEBUG
  while (ETH->MACMIIAR & ETH_MACMIIAR_MB) {}
#endif
  return ETH->MACMIIDR;
}

void eth_reset() {
  // Reset the MAC
  RCC->AHB1RSTR |= RCC_AHB1RSTR_ETHMACRST;

  // Reset the DMA controller
  ETH->DMABMR |= ETH_DMABMR_SR;
  while (ETH->DMABMR & ETH_DMABMR_SR) {}

  // Reset the PHY
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_BCR, ETH_PHY_BCR_SR);
#ifndef DEBUG
  while (eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_BCR) & ETH_PHY_BCR_SR) {}
#endif
}

void eth_init_descriptor_lists() {
  for (int i = 0; i < ETH_TX_BUFFER_NUM; i++) {
    // Set interrupt on frame completion and use chain descriptor lists
    TXDL[i].DES0 |= ETH_TDES0_IC | ETH_TDES0_TCH;
    TXDL[i].DES1 = 0;
    // Set buffer address and next descriptor list address
    TXDL[i].DES2 = (uint32_t)&TXB[i];
    TXDL[i].DES3 = (uint32_t)&TXDL[i + 1];
  }

  for (int i = 0; i < ETH_RX_BUFFER_NUM; i++) {
    // Receive descriptor is initially owned by DMA
    RXDL[i].DES0 |= ETH_RDES0_OWN;
    // Use chained structure and set buffer size
    RXDL[i].DES1 |= ETH_RDES0_RCH | (ETH_RX_BUFFER_SIZE & ETH_RDES1_RBS1);
    RXDL[i].DES2 = (uint32_t)&RXB[i];
    RXDL[i].DES3 = (uint32_t)&RXDL[i + 1];
  }

  // Loop back to complete chain
  TXDL[ETH_TX_BUFFER_NUM - 1].DES3 = (uint32_t)&TXDL[0];
  RXDL[ETH_RX_BUFFER_NUM - 1].DES3 = (uint32_t)&RXDL[0];

  // Set current to be initial descriptor
  currTXD = &TXDL[0];
  currRXD = &RXDL[0];

  // Set up descriptor lists in DMARDLAR and DMATDLAR
  ETH->DMATDLAR = (uint32_t)TXDL;
  ETH->DMARDLAR = (uint32_t)RXDL;
}

void eth_init() {
  // Enable GPIO pins for RMII
  gpio_afpin_init(ETH_RMII_REF_CLK, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_RMII_RXD0, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_RMII_RXD1, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_RMII_TX_EN, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_RMII_TXD0, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_RMII_TXD1, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_MDIO, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_MDC, GPIO_VHIGHSPEED);
  gpio_afpin_init(ETH_MII_CRS, GPIO_VHIGHSPEED);

  // Select RMII
  SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;

  // Enable ethernet TX/RX and MAC clock
  RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN | RCC_AHB1ENR_ETHMACEN;

  // Perform a software reset
  eth_reset();

  // Set PHY auto-negotiation advertisement types
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_ANAR,
                ETH_PHY_ANAR_100BASETXFD | ETH_PHY_ANAR_100BASETX | ETH_PHY_ANAR_10BASETXFD |
                    ETH_PHY_ANAR_10BASETX | ETH_PHY_ANAR_SELDEFAULT);

  // Enable auto-negotiation and its interrupt
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_BCR, ETH_PHY_BCR_ANEN);
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_IMR, ETH_PHY_IMR_ANC);

#ifndef DEBUG
  // Wait for auto-negotiation to finish
  while (!(eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_ISR) & ETH_PHY_IMR_ANC)) {}
#endif

#ifndef DEBUG
  uint16_t speed = (eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_SCSR) & ETH_PHY_SCSR_SPEED) >>
                   ETH_PHY_SCSR_SPEEDSHIFT;
#else
  uint16_t speed = ETH_PHY_100BASETXFD;
#endif

  uint32_t full_duplex = speed == ETH_PHY_10BASETXFD || speed == ETH_PHY_100BASETXFD;
  uint32_t fast        = speed == ETH_PHY_100BASETX || speed == ETH_PHY_100BASETXFD;

  // Set duplex mode and fast ethernet status
  ETH->MACCR |= (full_duplex << ETH_MACCR_DMSHIFT) | (fast << ETH_MACCR_FESSHIFT);

  // Enable store-and-forward
  ETH->DMAOMR |= ETH_DMAOMR_RSF | ETH_DMAOMR_TSF;

  eth_init_descriptor_lists();

  // Use AXI4 aligned beats and set beat length to 32
  ETH->DMABMR |= ETH_DMABMR_AAB;
  ETH->DMABMR &= ~ETH_DMABMR_PBL;
  ETH->DMABMR |= 32 << ETH_DMABMR_PBLSHIFT;

  // Enable normal DMA interrupts
  ETH->DMAIER |= ETH_DMAIER_NISE;
  // Set MAC filter to be promiscuous for now
  ETH->MACFFR |= ETH_MACFFR_PM;

  // Enable interrupt and set priority to 9, i.e. just below systick
  nvic_set_priority(NVIC_IRQ_ETH, 9);
  nvic_enable_irq(NVIC_IRQ_ETH);

  // Enable MAC TX/RX
  ETH->MACCR |= ETH_MACCR_TE | ETH_MACCR_RE;

  // Start DMA TX/RX
  ETH->DMAOMR |= ETH_DMAOMR_ST | ETH_DMAOMR_SR;
}

void eth_on_receive_frame(eth_receive_frame_cb_t cb) {
  _eth_receive_frame_cb = cb;
}

eth_err_rx_t eth_receive_frame() {
  eth_err_rx_t err = ETH_ERR_NONE;
  // If owned by DMA, then it's empty
  if (currRXD->DES0 & ETH_RDES0_OWN) {
    err = ETH_ERR_EMPTY;
    goto eth_rx_done;
  }

  // A valid 802.3 packet should fit into one ethernet frame
  if (!((currRXD->DES0 & ETH_RDES0_FS) && (currRXD->DES0 & ETH_RDES0_LS))) {
    err = ETH_ERR_INVALID_FRAME;
    goto eth_rx_invalid_frame;
  }

  // If there was an error in the summary
  if (currRXD->DES0 & ETH_RDES0_ES) {
    err = ETH_ERR_INVALID_FRAME;
    goto eth_rx_invalid_frame;
  }

  uint32_t length = (currRXD->DES0 & ETH_RDES0_FL) >> ETH_RDES0_FLSHIFT;
  uint8_t temp[ETH_TX_BUFFER_SIZE];
  memcpy(temp, (uint8_t *)currRXD->DES2, length);
  if (_eth_receive_frame_cb) _eth_receive_frame_cb(temp, length);

eth_rx_invalid_frame:
  // Give the invalid descriptor back to the DMA to use and move on
  currRXD->DES0 = ETH_RDES0_OWN;
  currRXD       = (eth_des_t *)currRXD->DES3;
eth_rx_done:
  // Mark receive buffer available and demand repoll
  ETH->DMASR &= ~ETH_DMASR_RBUS;
  ETH->DMARPDR = 0;
  return err;
}

void _eth_handler() {
  // If we have a receive event
  if (ETH->DMASR & ETH_DMASR_RS) {
    ETH->DMASR &= ~ETH_DMASR_RS;
    _eth_received_frame = 1;
  }
}
