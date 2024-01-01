#pragma once

#include "stm32f7.h"

#define FLASH_ACR_PRFTEN (1UL << 8)

typedef volatile struct {
  uint32_t ACR;
  uint32_t KEYR;
  uint32_t OPTKEYR;
  uint32_t SR;
  uint32_t CR;
  uint32_t OPTCR;
  uint32_t OPTCR1;
} flash_reg_t;

#define FLASH ((flash_reg_t *)FLASH_BASE)
