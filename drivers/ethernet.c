#include "ethernet.h"
#include "config.h"
#include "gpio.h"
#include "nvic.h"
#include "rcc.h"
#include "syscfg.h"

static __attribute__((aligned(4))) eth_des_t TXDL[ETH_TXDL_SIZE];
static __attribute__((aligned(4))) eth_des_t RXDL[ETH_RXDL_SIZE];
int received = 0;

void eth_phy_write(uint8_t address, uint8_t reg, uint16_t value) {
  // Note we implicitly have clock range to be HCLK / 42 <= 2.5MHz
  ETH->MACMIIDR = value;
  ETH->MACMIIAR |= (address << ETH_MACMIIAR_PASHIFT) |
                   (reg << ETH_MACMIIAR_MRSHIFT) | ETH_MACMIIAR_MW;

  // Wait until write finished
  // while (ETH->MACMIIAR & ETH_MACMIIAR_MB) {}
}

uint16_t eth_phy_read(uint8_t address, uint8_t reg) {
  ETH->MACMIIAR |=
      (address << ETH_MACMIIAR_PASHIFT) | (reg << ETH_MACMIIAR_MRSHIFT);
  // while (ETH->MACMIIAR & ETH_MACMIIAR_MB) {}
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
  // while (eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_BCR) & ETH_PHY_BCR_SR) {}
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
  RCC->AHB1ENR |=
      RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN | RCC_AHB1ENR_ETHMACEN;

  // Perform a software reset
  eth_reset();

  // Set PHY auto-negotiation advertisement types
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_ANAR,
                ETH_PHY_ANAR_100BASETXFD | ETH_PHY_ANAR_100BASETX |
                    ETH_PHY_ANAR_10BASETXFD | ETH_PHY_ANAR_10BASETX |
                    ETH_PHY_ANAR_SELDEFAULT);

  // Enable auto-negotiation and its interrupt
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_BCR, ETH_PHY_BCR_ANEN);
  eth_phy_write(ETH_PHY_ADDR_DEFAULT, ETH_PHY_IMR, ETH_PHY_IMR_ANC);

  // Wait for auto-negotiation to finish
  // while (!(eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_ISR) &
  // ETH_PHY_IMR_ANC)) {
  // }

  uint16_t speed = ETH_PHY_100BASETXFD; /*
       (eth_phy_read(ETH_PHY_ADDR_DEFAULT, ETH_PHY_SCSR) & ETH_PHY_SCSR_SPEED)
       >> ETH_PHY_SCSR_SPEEDSHIFT;*/

  uint32_t full_duplex =
      speed == ETH_PHY_10BASETXFD || speed == ETH_PHY_100BASETXFD;
  uint32_t fast = speed == ETH_PHY_100BASETX || speed == ETH_PHY_100BASETXFD;

  // Set duplex mode and fast ethernet status
  ETH->MACCR |=
      (full_duplex << ETH_MACCR_DMSHIFT) | (fast << ETH_MACCR_FESSHIFT);

  // Enable store-and-forward
  ETH->DMAOMR |= ETH_DMAOMR_RSF | ETH_DMAOMR_TSF;

  // Use AXI4 aligned beats and set beat length to 32
  ETH->DMABMR |= ETH_DMABMR_AAB;
  ETH->DMABMR &= ~ETH_DMABMR_PBL;
  ETH->DMABMR |= 32 << ETH_DMABMR_PBLSHIFT;

  // Enable normal DMA interrupts
  ETH->DMAIER |= ETH_DMAIER_NISE;

  // Set up descriptor lists in DMARDLAR and DMATDLAR
  ETH->DMATDLAR = (uint32_t)TXDL;
  ETH->DMARDLAR = (uint32_t)RXDL;

  // Set MAC filter to be promiscuous for now
  ETH->MACFFR |= ETH_MACFFR_PM;

  // Enabel interrupt and set priority to 5, i.e. just above systick
  nvic_set_priority(NVIC_IRQ_ETH, 5);
  nvic_enable_irq(NVIC_IRQ_ETH);

  // Enable MAC TX/RX
  ETH->MACCR |= ETH_MACCR_TE | ETH_MACCR_RE;

  // Start DMA TX/RX
  ETH->DMAOMR |= ETH_DMAOMR_ST | ETH_DMAOMR_SR;
}

void _eth_handler() {
  if (ETH->DMASR & ETH_DMASR_RS) received = 1;
}
