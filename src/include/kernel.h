#pragma once

#include <stddef.h>
#include <stdint.h>
#include "list.h"
#include "stm32f7.h"

#define MAX_PRIORITIES (5)

typedef void (*task_func_t)();

typedef struct {
  uint32_t *sp;
  size_t stack_size;
  task_func_t func;
  uint32_t priority;
  list_item_t list_item;
} task_t;

void task_init(task_t *task, task_func_t func, uint32_t priority, uint32_t *stack,
               size_t stack_size);
void kernel_init(uint32_t cycles_per_tick, uint32_t ticks_per_task);
