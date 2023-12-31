#include "gpio.h"
#include "rcc.h"

void gpio_pin_init(gpio_pin_t pin) {
  // Enable the GPIO bank in AHB
  RCC->AHB1ENR |= 1 << PINBANK(pin);

  // Support setting an alternate function on init
  if (!PINAF(pin)) return;
  gpio_pin_set_alt_func(pin, PINAF(pin));
  gpio_pin_set_mode(pin, GPIO_AF);
}

void gpio_pin_set_mode(gpio_pin_t pin, gpio_mode_t mode) {
  gpio_reg_t *GPIOx = GPIO(PINBANK(pin));
  uint32_t pos      = PINNUM(pin) << 1;
  GPIOx->MODER &= ~(0x3UL << pos);
  GPIOx->MODER |= mode << pos;
}

void gpio_pin_set_output_type(gpio_pin_t pin, gpio_output_type_t type) {
  gpio_reg_t *GPIOx = GPIO(PINBANK(pin));
  GPIOx->OTYPER &= ~(0x1UL << PINNUM(pin));
  GPIOx->OTYPER |= type << PINNUM(pin);
}

void gpio_pin_set_speed(gpio_pin_t pin, gpio_output_speed_t speed) {
  gpio_reg_t *GPIOx = GPIO(PINBANK(pin));
  uint32_t pos      = PINNUM(pin) << 1;
  GPIOx->OSPEEDR &= ~(0x3UL << pos);
  GPIOx->OSPEEDR |= speed << pos;
}

void gpio_pin_set_pupd(gpio_pin_t pin, gpio_pupd_t pupd) {
  gpio_reg_t *GPIOx = GPIO(PINBANK(pin));
  uint32_t pos      = PINNUM(pin) << 1;
  GPIOx->OSPEEDR &= ~(0x3UL << pos);
  GPIOx->OSPEEDR |= pupd << pos;
}

void gpio_pin_set_alt_func(gpio_pin_t pin, uint8_t af) {
  gpio_reg_t *GPIOx = GPIO(PINBANK(pin));

  uint32_t pos           = PINNUM(pin) & 0x7UL;
  volatile uint32_t *reg = PINNUM(pin) < 8 ? &GPIOx->AFLR : &GPIOx->AFHR;
  *reg &= ~(0xFUL << pos);
  *reg |= af << pos;
}
