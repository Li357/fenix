#include <stdio.h>
#include "uart.h"
#include "systick.h"
#include "util.h"

int main() {
  systick_init(CLOCK_SPEED); // tick every second

  usart_cfg_t cfg = {
    .peripheral = USART_1,
    .baudrate = 9600,
  };

  uart_usart_init(USART_1);
  usart_peri_init(&cfg);

  char *msg = "Hello World!\n";

  while (1) {
    delay(1); // log every second
    puts(msg);
  }

  return 0;
}
