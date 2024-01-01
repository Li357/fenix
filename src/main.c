#include <stddef.h>
#include <stdio.h>
#include "ethernet.h"
#include "systick.h"
#include "usart.h"
#include "util.h"

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");
  printf("MMC: %d, PTPT: %d, DMABMR: %d\n", offsetof(struct eth_reg_t, _MMC),
         offsetof(struct eth_reg_t, _PTPT), offsetof(struct eth_reg_t, DMABMR));

  eth_init();

  while (1) {
    if (received) {
      puts("Ethernet frame received!\n");
      received = 0;
    }
  }

  return 0;
}
