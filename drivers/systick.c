#include "systick.h"
#include "rcc.h"

volatile uint32_t _systicks;

void systick_init(uint32_t ticks) {
  // SysTick is 24-bit
  SYST->RVR = (ticks - 1) & 0xFFFFFFUL;
  SYST->CVR = 0;
  SYST->CSR |= SYST_CSR_ENABLE | SYST_CSR_TICKINT;
  _systicks = 0;
}

void _systick_handler() {
  _systicks++;
}
