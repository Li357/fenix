#include "rcc.h"
#include "uart.h"
#include "gpio.h"

void uart_usart_init(uart_usart_peri_t peripheral) {
  switch (peripheral) {
    case USART_2:
    case USART_3:
    case UART_4:
    case UART_5:
    case UART_7:
    case UART_8:
      rRCC->APB1ENR |= 1 << peripheral;
      break;
    case USART_1:
    case USART_6:
      rRCC->APB2ENR |= 1 << peripheral;
    default: break;
  }
}

void usart_peri_init(usart_cfg_t *config) {
  gpio_cfg_t gpio_cfg = {
    .bank = GPIO_A,
    .port = GPIO_8,
    .mode = GPIO_AF,
    .af = GPIO_AF7,
  };
  gpio_port_init(&gpio_cfg);
  gpio_cfg.port = GPIO_9;
  gpio_port_init(&gpio_cfg);
  gpio_cfg.port = GPIO_10;
  gpio_port_init(&gpio_cfg);
  gpio_cfg.port = GPIO_11;
  gpio_port_init(&gpio_cfg);

  uint16_t uartdiv = CLOCK_SPEED / config->baudrate;
  rUSART1->BRR = ((uartdiv / 16) << 4UL | (uartdiv % 16));

  rUSART1->CR1 |= 0x9UL;
}

void usart_transmit(char *ptr, int len) {
  while (!(rUSART1->ISR & 0x40UL));
  for (int i = 0; i < len; i++, ptr++)
    rUSART1->TDR = *ptr;
}
