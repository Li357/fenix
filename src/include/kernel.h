#pragma once

#include <stddef.h>
#include <stdint.h>
#include "list.h"
#include "stm32f7.h"

#define MAX_PRIORITIES           (5)
#define KERNEL_DEFAULT_EPSR      (0x01000000UL)
#define KERNEL_DEFAULT_EXCRETURN (0xFFFFFFFDUL)

typedef void (*task_func_t)(void *task_param);

typedef enum {
  TASK_SUSPENDED,
  TASK_READY,
} task_state_t;

typedef struct {
  uint32_t *sp;
  size_t stack_size;
  task_func_t func;
  uint32_t priority;
  task_state_t state;
  list_item_t list_item;
} task_t;
void task_init(task_t *task, task_func_t func, void *task_param, uint32_t priority, uint32_t *stack,
               size_t stack_size);
void task_setready(task_t *task);
void task_suspend();
void task_yield();
void kernel_init();
void kernel_start();
void kernel_delay();
