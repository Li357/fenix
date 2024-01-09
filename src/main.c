#include <stddef.h>
#include <stdio.h>
#include "kernel.h"
#include "net.h"
#include "socket.h"
#include "systick.h"
#include "usart.h"

uint32_t task_eth_stack[500];
task_t task_eth;
void task_eth_func(void *param) {
  while (1) {
    uint8_t frame[ETH_RX_BUFFER_SIZE];
    uint32_t length;

    while (eth_receive_frame(frame, &length) != ETH_ERR_EMPTY) { eth_process(frame, length); }
    task_suspend();
  }
}

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  kernel_init();

  task_init(&task_eth, task_eth_func, NULL, 0, task_eth_stack, 100);
  int fd = socket_init(80);
  socket_listen(fd);

  eth_init();
  kernel_start();

  return 0;
}
