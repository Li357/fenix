#include <stddef.h>
#include <stdio.h>
#include "ethernet.h"
#include "kernel.h"
#include "systick.h"
#include "usart.h"

uint32_t task_eth_stack[100];
task_t task_eth;
void task_eth_func(void *param) {
  while (1) {
    uint8_t frame[ETH_RX_BUFFER_SIZE];
    uint32_t length;

    while (eth_receive_frame(frame, &length) != ETH_ERR_EMPTY) {
      puts("Frame: ");
      for (size_t i = 0; i < length; i++) printf("%02x", frame[i]);
      puts("\n");
    }
    task_suspend();
  }
}

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  kernel_init();

  task_init(&task_eth, task_eth_func, NULL, 0, task_eth_stack, 100);

  eth_init();
  kernel_start();

  return 0;
}
