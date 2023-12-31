#include <stdio.h>

#include "systick.h"
#include "usart.h"
#include "util.h"

int main() {
  systick_init(CLOCK_SPEED / 1000);  // tick every ms

  usart_init(USART1, 115200);

  char *msg = "Hello World!\n";
  puts(msg);

  while (1) {
    delay(1000);  // log every second
    puts(msg);
  }

  return 0;
}
