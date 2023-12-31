#include <stdio.h>
#include "usart.h"
#include "systick.h"
#include "util.h"

int main() {
  //systick_init(CLOCK_SPEED); // tick every second

  usart_init(USART1, 115200);

  char *msg = "Hello World!\n";
  puts(msg);

  while (1);/* {
    //delay(1); // log every second
    puts(msg);
  }*/

  return 0;
}
