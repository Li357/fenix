#pragma once

#include "stm32f7.h"

#define RCC_CR_PLLRDY          (1UL << 25)
#define RCC_CR_PLLON           (1UL << 24)
#define RCC_PLLCFGR_PLLPSHIFT  (16)
#define RCC_PLLCFGR_PLLP       (0x3UL << RCC_PLLCFGR_PLLPSHIFT)
#define RCC_PLLCFGR_PLLNSHIFT  (6)
#define RCC_PLLCFGR_PLLN       (0x1FFUL << RCC_PLLCFGR_PLLNSHIFT)
#define RCC_PLLCFGR_PLLMSHIFT  (0)
#define RCC_PLLCFGR_PLLM       (0x1FUL << RCC_PLLCFGR_PLLMSHIFT)
#define RCC_CFGR_PPRE2SHIFT    (13)
#define RCC_CFGR_PPRE2         (0x7UL << RCC_CFGR_PPRE2SHIFT)
#define RCC_CFGR_PPRE1SHIFT    (10)
#define RCC_CFGR_PPRE1         (0x7UL << RCC_CFGR_PPRE1SHIFT)
#define RCC_CFGR_HPRESHIFT     (4)
#define RCC_CFGR_HPRE          (0xFUL << RCC_CFGR_HPRESHIFT)
#define RCC_AHB1ENR_ETHMACRXEN (1UL << 27)
#define RCC_AHB1ENR_ETHMACTXEN (1UL << 26)
#define RCC_AHB1ENR_ETHMACEN   (1UL << 25)
#define RCC_APB2ENR_SYSCFGEN   (1UL << 14)

typedef volatile struct {
  uint32_t CR;
  uint32_t PLLCFGR;
  uint32_t CFGR;
  uint32_t CIR;
  uint32_t AHB1RSTR;
  uint32_t AHB2RSTR;
  uint32_t AHB3RSTR;
  uint32_t _RESERVED0;
  uint32_t APB1RSTR;
  uint32_t APB2RSTR;
  uint32_t _RESERVED1[2];
  uint32_t AHB1ENR;
  uint32_t AHB2ENR;
  uint32_t AHB3ENR;
  uint32_t _RESERVED2;
  uint32_t APB1ENR;
  uint32_t APB2ENR;
  uint32_t _RESERVED3[2];
  uint32_t AHB1LPENR;
  uint32_t AHB2LPENR;
  uint32_t AHB3LPENR;
  uint32_t _RESERVED4;
  uint32_t APB1LPENR;
  uint32_t APB2LPENR;
  uint32_t _RESERVED5[2];
  uint32_t BDCR;
  uint32_t CSR;
  uint32_t _RESERVED6[2];
  uint32_t SSCGR;
  uint32_t PLLI2SCFGR;
  uint32_t PLLSAICFGR;
  uint32_t DCKCFGR1;
  uint32_t DCKCFGR2;
} rcc_reg_t;

#define RCC ((rcc_reg_t *)RCC_BASE)

