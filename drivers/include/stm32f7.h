#pragma once

#include <stdint.h>

#define CLOCK_SPEED    (216000000)

/* Memory map */
#define APB1_BASE      (0x40000000UL)
#define USART2_BASE    (APB1_BASE + 0x4400UL)
#define USART3_BASE    (APB1_BASE + 0x4800UL)
#define UART4_BASE     (APB1_BASE + 0x4C00UL)
#define UART5_BASE     (APB1_BASE + 0x5000UL)
#define UART7_BASE     (APB1_BASE + 0x7800UL)
#define UART8_BASE     (APB1_BASE + 0x7C00UL)

#define APB2_BASE      (0x40010000UL)
#define USART1_BASE    (APB2_BASE + 0x1000UL)
#define USART6_BASE    (APB2_BASE + 0x1400UL)

#define AHB1_BASE      (0x40020000UL)
#define GPIOA_BASE     (AHB1_BASE + 0UL)
#define GPIOB_BASE     (AHB1_BASE + 0x400UL)
#define GPIOC_BASE     (AHB1_BASE + 0x800UL)
#define GPIOD_BASE     (AHB1_BASE + 0xC00UL)
#define GPIOE_BASE     (AHB1_BASE + 0x1000UL)
#define GPIOF_BASE     (AHB1_BASE + 0x1400UL)
#define GPIOG_BASE     (AHB1_BASE + 0x1800UL)
#define GPIOH_BASE     (AHB1_BASE + 0x1C00UL)
#define GPIOI_BASE     (AHB1_BASE + 0x2000UL)
#define GPIOJ_BASE     (AHB1_BASE + 0x2400UL)
#define GPIOK_BASE     (AHB1_BASE + 0x2800UL)

#define RCC_BASE       (AHB1_BASE + 0x3800UL)

#define PPB_BASE       (0xE000E000UL)
#define SYST_BASE      (PPB_BASE + 0x10UL)
