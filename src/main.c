#include <stddef.h>
#include <stdio.h>
#include "kernel.h"
#include "net.h"
#include "socket.h"
#include "systick.h"
#include "usart.h"

task_t socket_task;
uint32_t socket_task_stack[500];
void socket_task_func(void *param) {
  int sd = socket_open(1337);
  socket_listen(sd);
  while (1) {
    if (socket_accept(sd)) { puts("here!"); }
  }
  task_yield();
}

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  kernel_init();

  net_init();
  eth_init();

  task_init(&socket_task, socket_task_func, NULL, 0, socket_task_stack, 500);
  task_setready(&socket_task);

  kernel_start();

  return 0;
}
