#include "rcc.h"
#include "gpio.h"

void gpio_init(gpio_bank_t bank) {
  rRCC->AHB1ENR |= 1 << bank;
}

void gpio_port_init(gpio_cfg_t *cfg) {
  gpio_reg_t *GPIOx = rGPIO(cfg->bank);
  uint32_t pos = cfg->port << 1;

  GPIOx->MODER &= ~(0x3UL << pos);
  GPIOx->MODER |= cfg->mode << pos;

  GPIOx->OTYPER &= ~(0x1UL << cfg->port);
  GPIOx->OTYPER |= cfg->output_type << cfg->port;

  GPIOx->OSPEEDR &= ~(0x3UL << pos);
  GPIOx->OSPEEDR |= cfg->output_speed << pos;

  GPIOx->PUPDR &= ~(0x3UL << pos);
  GPIOx->PUPDR |= cfg->pupd << pos;

  pos = cfg->port & 0x7UL;
  if (cfg->port < 8) {
    GPIOx->AFLR &= ~(0xFUL << pos);
    GPIOx->AFLR |= cfg->af << pos;
    return;
  }
  GPIOx->AFHR &= ~(0xFUL << pos);
  GPIOx->AFHR |= cfg->af << pos;
}
