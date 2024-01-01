#pragma once

#include "stm32f7.h"

#define SYSCFG_PMC_MII_RMII_SEL (1UL << 23)

typedef volatile struct {
  uint32_t MEMRMP;
  uint32_t PMC;
  uint32_t EXTICR1;
  uint32_t EXTICR2;
  uint32_t EXTICR3;
  uint32_t EXTICR4;
  uint32_t CMPCR;
} syscfg_reg_t;

#define SYSCFG ((syscfg_reg_t *)SYSCFG_BASE)
