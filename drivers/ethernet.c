#include "ethernet.h"
#include "config.h"
#include "gpio.h"
#include "rcc.h"

void eth_reset() {
  ETH->DMABMR |= ETH_DMABMR_SR;
  while (ETH->DMABMR & ETH_DMABMR_SR) {}
}

void eth_init() {
  // Enable GPIO pins for RMII
  gpio_pin_init(ETH_RMII_REF_CLK);
  gpio_pin_init(ETH_RMII_RXD0);
  gpio_pin_init(ETH_RMII_RXD1);
  gpio_pin_init(ETH_RMII_TX_EN);
  gpio_pin_init(ETH_RMII_TXD0);
  gpio_pin_init(ETH_RMII_RXD1);
  gpio_pin_init(ETH_MDIO);
  gpio_pin_init(ETH_MDC);
  gpio_pin_init(ETH_MII_CRS);
  gpio_pin_init(ETH_MII_RX_ER);

  // Enable ethernet TX/RX and MAC clock
  RCC->AHB1ENR |=
      RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN | RCC_AHB1ENR_ETHMACEN;

  // Perform a software reset
  eth_reset();

  // Talk to PHY via RMII to set bus parameters
  // Set DMABMR according to PHY
  // Set up descriptor lists in DMARDL/HAR
  // Set MAC filter registers
  // Set MACCR with PHY parameters and enable TX/RX
  // Set DMAOMR to start transmission
  // Handle interrupts from descriptor lists
}
