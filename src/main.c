#include <stddef.h>
#include <stdio.h>
#include "ethernet.h"
#include "systick.h"
#include "usart.h"
#include "util.h"

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  eth_init();

  while (1) {
    delay(1000);
    if (_eth_received_frame) {
      while (eth_receive_frame() != ETH_ERR_EMPTY) {}
      _eth_received_frame = 0;
    }
  }

  return 0;
}
