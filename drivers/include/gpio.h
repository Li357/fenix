#pragma once

#include "stm32f7.h"

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
  
#define rGPIO_ADDR(x) ((gpio_reg_t *)(x))
#define rGPIOA        (rGPIO_ADDR(GPIOA_BASE))
#define rGPIOB        (rGPIO_ADDR(GPIOB_BASE))
#define rGPIOC        (rGPIO_ADDR(GPIOC_BASE))
#define rGPIOD        (rGPIO_ADDR(GPIOD_BASE))
#define rGPIOE        (rGPIO_ADDR(GPIOE_BASE))
#define rGPIOF        (rGPIO_ADDR(GPIOF_BASE))
#define rGPIOG        (rGPIO_ADDR(GPIOG_BASE))
#define rGPIOH        (rGPIO_ADDR(GPIOH_BASE))
#define rGPIOI        (rGPIO_ADDR(GPIOI_BASE))
#define rGPIOJ        (rGPIO_ADDR(GPIOJ_BASE))
#define rGPIOK        (rGPIO_ADDR(GPIOK_BASE))
#define rGPIO(x)      (rGPIO_ADDR(AHB1_BASE + (x) * 0x400UL))

typedef enum {
  GPIO_A,
  GPIO_B,
  GPIO_C,
  GPIO_D,
  GPIO_E,
  GPIO_F,
  GPIO_G,
  GPIO_H,
  GPIO_I,
  GPIO_J,
  GPIO_K,
} gpio_bank_t;

typedef enum {
  GPIO_0,
  GPIO_1,
  GPIO_2,
  GPIO_3,
  GPIO_4,
  GPIO_5,
  GPIO_6,
  GPIO_7,
  GPIO_8,
  GPIO_9,
  GPIO_10,
  GPIO_11,
  GPIO_12,
  GPIO_13,
  GPIO_14,
  GPIO_15,
} gpio_port_t;

typedef enum {
  GPIO_AF0,
  GPIO_AF1,
  GPIO_AF2,
  GPIO_AF3,
  GPIO_AF4,
  GPIO_AF5,
  GPIO_AF6,
  GPIO_AF7,
  GPIO_AF8,
  GPIO_AF9,
  GPIO_AF10,
  GPIO_AF11,
  GPIO_AF12,
  GPIO_AF13,
  GPIO_AF14,
  GPIO_AF15,
} gpio_af_t;

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

typedef struct {
  gpio_bank_t bank;
  gpio_port_t port;
  gpio_mode_t mode;
  gpio_output_type_t output_type;
  gpio_output_speed_t output_speed;
  gpio_pupd_t pupd;
  gpio_af_t af;
} gpio_cfg_t;

void gpio_init(gpio_bank_t bank);
void gpio_port_init(gpio_cfg_t *cfg);
