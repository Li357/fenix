#include <stddef.h>
#include <stdio.h>
#include "ethernet.h"
#include "kernel.h"
#include "systick.h"
#include "usart.h"

void on_receive_frame(uint8_t *frame, size_t len) {
  puts("Frame: ");
  for (size_t i = 0; i < len; i++, frame++) printf("%02x", *frame);
  puts("\n");
}

int main() {
  usart_init(USART1, 115200);

  puts("Hello from Fenix!\n");

  // eth_init();
  // eth_on_receive_frame(on_receive_frame);

  kernel_init(1000, 10);

  /* while (1) {
    if (_eth_received_frame) {
      while (eth_receive_frame() != ETH_ERR_EMPTY) {}
      _eth_received_frame = 0;
    }
  } */

  return 0;
}
