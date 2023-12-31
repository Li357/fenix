#pragma once

#include "stm32f7.h"

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
