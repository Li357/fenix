#include "mutex.h"

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

void mutex_init(mutex_t *mutex) {
  *mutex = 0;
}

void mutex_acquire(mutex_t *mutex) {
  uint32_t state;
  do {
    state = _ldrex(mutex);
    state++;
  } while (_strex(mutex, state));
}

void mutex_release(mutex_t *mutex) {
  uint32_t state;
  do {
    state = _ldrex(mutex);
    state--;
  } while (_strex(mutex, state));
}

