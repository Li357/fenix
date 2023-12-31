#include "rcc.h"
#include "usart.h"
#include "gpio.h"

usart_reg_t *USART1 = ((usart_reg_t *)USART1_BASE);
usart_reg_t *USART2 = ((usart_reg_t *)USART2_BASE);
usart_reg_t *USART3 = ((usart_reg_t *)USART3_BASE);
usart_reg_t *UART4  = ((usart_reg_t *)UART4_BASE);
usart_reg_t *UART5  = ((usart_reg_t *)UART5_BASE);
usart_reg_t *USART6 = ((usart_reg_t *)USART6_BASE);
usart_reg_t *UART7  = ((usart_reg_t *)UART7_BASE);
usart_reg_t *UART8  = ((usart_reg_t *)UART8_BASE);

void usart_init(usart_reg_t *usart, uint32_t baudrate) {
  // Enable APB clock and setup GPIO pins
  if (usart == USART1) USART_INIT(USART1, RCC->APB2ENR);
  if (usart == USART2) USART_INIT(USART2, RCC->APB1ENR);
  if (usart == USART3) USART_INIT(USART3, RCC->APB1ENR);
  if (usart == UART4)  UART_INIT(UART4, RCC->APB1ENR);
  if (usart == UART5)  UART_INIT(UART5, RCC->APB1ENR);
  if (usart == USART6) USART_INIT(USART6, RCC->APB2ENR);
  if (usart == UART7)  UART_INIT(UART7, RCC->APB1ENR);
  if (usart == UART8)  UART_INIT(UART8, RCC->APB1ENR);
  
  // Calculate division rate
  uint16_t uartdiv = CLOCK_SPEED / baudrate;
  usart->BRR = ((uartdiv / 16) << USART_BRR_QSHIFT | (uartdiv % 16));

  // Enable USART transmit
  usart->CR1 |= 0x9UL;//USART_CR1_TE | USART_CR1_UE;
}

void usart_transmit(usart_reg_t *usart, char *ptr, int len) {
  while (!(usart->ISR & USART_ISR_TXE));
  for (int i = 0; i < len; i++, ptr++)
    usart->TDR = *ptr;
}
