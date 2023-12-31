#pragma once

#include "gpio.h"

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



