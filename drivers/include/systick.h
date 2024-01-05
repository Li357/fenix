#pragma once

#include "stm32f7.h"

#define SYST_CSR_ENABLE  (1UL << 0)
#define SYST_CSR_TICKINT (1UL << 1)

typedef volatile struct {
  uint32_t CSR;
  uint32_t RVR;
  uint32_t CVR;
  uint32_t CALIB;
} systick_reg_t;

#define SYST ((systick_reg_t *)SYST_BASE)

void systick_init(uint32_t ticks);
