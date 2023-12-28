#include "gpio.h"

int main() {
  gpio_cfg_t cfg = {
    .bank = GPIO_B,
    .port = GPIO_1,
    .mode = GPIO_ANALOG,
  };

  gpio_init(GPIO_B);
  gpio_port_init(&cfg);

  while (1);

  return 0;
}
