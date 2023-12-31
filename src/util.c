#include "systick.h"
#include "util.h"

void delay(uint32_t ticks) {
  uint32_t until = _systicks + ticks;
  while (_systicks < until) {}
}
