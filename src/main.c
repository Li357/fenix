#include <stddef.h>
#include <stdio.h>
#include "ethernet.h"
#include "kernel.h"
#include "systick.h"
#include "usart.h"

/*void on_receive_frame(uint8_t *frame, size_t len) {
  puts("Frame: ");
  for (size_t i = 0; i < len; i++, frame++) printf("%02x", *frame);
  puts("\n");
}*/

void task_test() {
  while (1) {
    kernel_delay(100);
    puts("hi from HIGHER TASK!");
    task_yield();
  }
}

static uint32_t stack[100];

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  // eth_init();
  // eth_on_receive_frame(on_receive_frame);
  kernel_init();

  task_t task;
  task_init(&task, task_test, 0, stack, 100);
  task_setready(&task);

  kernel_start();
  /* while (1) {
    if (_eth_received_frame) {
      while (eth_receive_frame() != ETH_ERR_EMPTY) {}
      _eth_received_frame = 0;
    }
  } */

  return 0;
}
