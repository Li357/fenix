#pragma once

#include "stm32f7.h"

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

#define rUSART1 ((usart_reg_t *)(USART1_BASE))

// Bit numbers in the RCC APBx clock enable registers
typedef enum {
  USART_1 = 4,
  USART_2 = 17,
  USART_3 = 18,
  UART_4 = 19,
  UART_5 = 20,
  USART_6 = 5,
  UART_7 = 30,
  UART_8 = 31,
} uart_usart_peri_t;

typedef struct {
  uart_usart_peri_t peripheral;
  uint16_t baudrate;
} usart_cfg_t;

void uart_usart_init(uart_usart_peri_t peripheral);
void usart_peri_init(usart_cfg_t *config);
void usart_transmit(char *ptr, int len);
