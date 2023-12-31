#pragma once

#include "stm32f7.h"

typedef volatile struct {
  uint32_t CSR;
  uint32_t RVR;
  uint32_t CVR;
  uint32_t CALIB;
} systick_reg_t;

#define SYST ((systick_reg_t *)SYST_BASE)

extern volatile uint32_t _systicks;

void systick_init(uint32_t ticks);
