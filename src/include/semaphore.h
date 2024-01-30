#pragma once

#include <stdint.h>

typedef uint32_t sem_t;

inline uint32_t _ldrex(volatile uint32_t *addr) {
  uint32_t result;
  asm volatile("ldrex %0, %1" : "=r"(result) : "m"(*addr));
  return result;
}

inline uint32_t _strex(volatile uint32_t *addr, uint32_t value) {
  uint32_t result;
  asm volatile("strex %0, %2, %1" : "=&r"(result), "=m"(*addr) : "r"(value));
  return result;
}
