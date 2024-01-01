#pragma once

#include "stm32f7.h"

#define PIN(bank, num)       (((bank - 'A') << 8) | (num))
#define AFPIN(bank, num, af) (PIN(bank, num) | ((af) << 4))
#define PINBANK(pin)         (pin >> 8)
#define PINAF(pin)           ((pin & 0xF0) >> 4)
#define PINNUM(pin)          (pin & 0xF)

typedef uint16_t gpio_pin_t;

typedef volatile struct {
  uint32_t MODER;
  uint32_t OTYPER;
  uint32_t OSPEEDR;
  uint32_t PUPDR;
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSR;
  uint32_t LCKR;
  uint32_t AFLR;
  uint32_t AFHR;
} gpio_reg_t;

#define GPIO(bank) ((gpio_reg_t *)(GPIOA_BASE + (bank) * 0x400UL))

typedef enum {
  GPIO_INPUT,
  GPIO_OUTPUT,
  GPIO_AF,
  GPIO_ANALOG,
} gpio_mode_t;

typedef enum {
  GPIO_PUSHPULL,
  GPIO_OPENDRAIN,
} gpio_output_type_t;

typedef enum {
  GPIO_LOWSPEED,
  GPIO_MEDSPEED,
  GPIO_HIGHSPEED,
  GPIO_VHIGHSPEED,
} gpio_output_speed_t;

typedef enum {
  GPIO_NOPUPD,
  GPIO_PU,
  GPIO_PD,
  GPIO_PUPDRES,
} gpio_pupd_t;

void gpio_pin_init(gpio_pin_t pin, gpio_mode_t mode, gpio_output_type_t type,
                   gpio_output_speed_t speed, gpio_pupd_t pupd);
void gpio_afpin_init(gpio_pin_t pin, gpio_output_speed_t speed);
