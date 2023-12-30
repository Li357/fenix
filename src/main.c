#include <stdio.h>
#include "uart.h"

int main() {
  usart_cfg_t cfg = {
    .peripheral = USART_1,
    .baudrate = 9600,
  };

  uart_usart_init(USART_1);
  usart_peri_init(&cfg);

  char *msg = "Hello World!\n";

  puts(msg);

  while (1);

  return 0;
}
