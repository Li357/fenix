#include "systick.h"
#include "rcc.h"

volatile uint32_t _systicks;

void systick_init(uint32_t ticks) {
  rSYST->RVR = (ticks - 1) & 0xFFFFFFUL;
  rSYST->CVR = 0;
  rSYST->CSR |= 3;
  _systicks = 0;
  rRCC->APB2ENR |= 0x4000UL;
}

void _systick_handler() { 
  _systicks++;
}
