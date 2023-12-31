#pragma once

#include "stm32f7.h"
#include "pindefs.h"

#define USART_ISR_TXE    (1UL << 7)
#define USART_BRR_QSHIFT (4UL)
#define USART_CR1_TE     (1UL << 3)
#define USART_CR1_UE     (1UL << 0)

#define USART1_CLK_BIT   (1UL << 4)
#define USART2_CLK_BIT   (1UL << 17)
#define USART3_CLK_BIT   (1UL << 18)
#define UART4_CLK_BIT    (1UL << 19)
#define UART5_CLK_BIT    (1UL << 20)
#define USART6_CLK_BIT   (1UL << 5)
#define UART7_CLK_BIT    (1UL << 30)
#define UART8_CLK_BIT    (1UL << 31)

#define UART_INIT(usart, apb) {                  \
    apb |= usart##_CLK_BIT;                      \
    gpio_pin_init(usart##_TX);                   \
    gpio_pin_init(usart##_RX);                   \
    gpio_pin_init(usart##_CTS);                  \
    gpio_pin_init(usart##_RTS);                  \
  } 

#define USART_INIT(usart, apb) {                 \
    UART_INIT(usart, apb);                       \
    gpio_pin_init(usart##_CK);                   \
  }

typedef volatile struct {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t CR3;
  uint32_t BRR;
  uint32_t GPTR;
  uint32_t RTOR;
  uint32_t RQR;
  uint32_t ISR;
  uint32_t ICR;
  uint32_t RDR;
  uint32_t TDR;
} usart_reg_t;

extern usart_reg_t *USART1;
extern usart_reg_t *USART2;
extern usart_reg_t *USART3;
extern usart_reg_t *UART4;
extern usart_reg_t *UART5;
extern usart_reg_t *USART6;
extern usart_reg_t *UART7;
extern usart_reg_t *UART8;

void usart_init(usart_reg_t *usart, uint32_t baudrate);
void usart_transmit(usart_reg_t *usart, char *ptr, int len);
