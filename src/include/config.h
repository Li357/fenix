#pragma once

#include "gpio.h"

/*
 * The STM32F756ZG has a max frequency of 216MHz, with
 * PLL_P, PLL_N, and PLL_M to scale the VCO:
 *
 * HSI    = 16MHz
 * f_VCOi = HSI / PLL_M <= 2MHz
 * f_VCOo = f_VCOi * PLL_N with 100MHz <= f_VCOo <= 432MHz
 * f_sys  = f_VCOo / PLL_P <= 216MHz with PLL_P = 2, 4, 6, 8
 *
 * We'll target 100MHz = HSI / 16 * 100 / 2 for now
 *
 */

#define PLL_M     (16)
#define PLL_N     (200)
#define PLL_P     (2)
#define SYS_CLOCK (BASE_CLOCK / PLL_M * PLL_N / PLL_P)

/*
 * We also have scalers for the AHB1/2, APB1/2 clocks:
 * - AHB1/2 <= 216MHz, APB2 <= 108MHz, APB1 <= 54MHz
 *
 * Ethernet on AHB requires >= 25MHz, so we'll keep them undivided, i.e. AHB_PRE
 * = 0 per the datasheet Then we'll let APB2 = AHB / 2 and APB1 = AHB / 4 to
 * save some power
 */

#define APB1_PRE   (0b101)
#define APB2_PRE   (0b100)
#define AHB_PRE    (0)
#define APB1_CLOCK (SYS_CLOCK / 4)
#define APB2_CLOCK (SYS_CLOCK / 2)
#define AHB_CLOCK  SYS_CLOCK

/* Pin definitions for peripherals */
#define USART1_CK  AFPIN('A', 8, 7)
#define USART1_TX  AFPIN('A', 9, 7)
#define USART1_RX  AFPIN('A', 10, 7)
#define USART1_CTS AFPIN('A', 11, 7)
#define USART1_RTS AFPIN('A', 12, 7)

#define USART2_CK  AFPIN('A', 4, 7)
#define USART2_TX  AFPIN('A', 2, 7)
#define USART2_RX  AFPIN('A', 3, 7)
#define USART2_CTS AFPIN('A', 0, 7)
#define USART2_RTS AFPIN('A', 1, 7)

#define USART3_CK  AFPIN('B', 12, 7)
#define USART3_TX  AFPIN('B', 10, 7)
#define USART3_RX  AFPIN('B', 11, 7)
#define USART3_CTS AFPIN('B', 13, 7)
#define USART3_RTS AFPIN('B', 14, 7)

#define UART4_TX   AFPIN('A', 0, 8)
#define UART4_RX   AFPIN('A', 1, 8)
#define UART4_CTS  AFPIN('B', 0, 8)
#define UART4_RTS  AFPIN('A', 15, 8)

#define UART5_TX   AFPIN('C', 12, 8)
#define UART5_RX   AFPIN('D', 2, 8)
#define UART5_CTS  AFPIN('C', 9, 7)
#define UART5_RTS  AFPIN('C', 8, 7)

#define USART6_CK  AFPIN('C', 8, 8)
#define USART6_TX  AFPIN('C', 6, 8)
#define USART6_RX  AFPIN('C', 7, 8)
#define USART6_CTS AFPIN('G', 13, 8)
#define USART6_RTS AFPIN('G', 12, 8)

#define UART7_TX   AFPIN('E', 8, 8)
#define UART7_RX   AFPIN('E', 7, 8)
#define UART7_CTS  AFPIN('E', 10, 8)
#define UART7_RTS  AFPIN('E', 9, 8)

#define UART8_TX   AFPIN('E', 1, 8)
#define UART8_RX   AFPIN('E', 0, 8)
#define UART8_CTS  AFPIN('D', 14, 8)
#define UART8_RTS  AFPIN('D', 15, 8)
