#include "systick.h"
#include "rcc.h"

volatile uint32_t _systicks;

void systick_init(uint32_t ticks) {
  SYST->RVR = (ticks - 1) & 0xFFFFFFUL;
  SYST->CVR = 0;
  SYST->CSR |= 3;
  _systicks = 0;
  RCC->APB2ENR |= 0x4000UL;
}

void _systick_handler() { 
  _systicks++;
}
