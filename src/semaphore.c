#include "semaphore.h"

inline void sem_init(sem_t *sem) {
  *sem = 0;
}

void sem_give(sem_t *sem) {
  uint32_t val;
  do {
    val = _ldrex(sem);
    val++;
  } while (_strex(sem, val));
}

void sem_take(sem_t *sem) {
  uint32_t val;
  do {
    val = _ldrex(sem);
    val--;
  } while (_strex(sem, val));
}
