#pragma once

#include <stdint.h>

typedef uint32_t mutex_t;

void mutex_init(mutex_t *mutex);
void mutex_acquire(mutex_t *mutex);
void mutex_release(mutex_t *mutex);
