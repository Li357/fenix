/*
 * A simple RTOS with preemptive context-switching (modeled after FreeRTOS)
 *
 * - Tasks are created with static stack size and given priority, and execute in
 *   thread mode
 * - Scheduler preempts in a number of ticks and always chooses the highest priority task
 *   at time of preemption
 *
 * - SVC is used to start the root idle task, whose stack is setup to look like a task that was
 *   context-switched out of and will be switched into
 * - SysTick increments ticks and calls a PendSV if a task needs to be preempted
 * - PendSV performs the actual context switch to the next task
 * - PendSV and SysTick have lowest priority to allow handlers to run after tasks are ready
 *
 */

#include "kernel.h"
#include <stddef.h>
#include "config.h"
#include "list.h"
#include "scb.h"
#include "stdio.h"
#include "sys/select.h"
#include "systick.h"

void kernel_start_root_task();

static volatile uint32_t _systicks;
static volatile uint32_t _systick_next_preempt;

static list_t _ready_tasklists[MAX_PRIORITIES];
static task_t *_curr_task;

static task_t _root_task;
static uint32_t _root_stack[KERNEL_DEFAULT_STACK_SIZE];

static void _root_task_func() {
  while (1) {
    kernel_delay(500);
    puts("hi! from idle!\n");
  }
}

void kernel_delay(uint32_t ticks) {
  uint32_t until = _systicks + ticks;
  while (_systicks < until) {}
}

void task_init_stack(task_t *task) {
  uint32_t *sp = task->sp;
  sp--;
  *sp = KERNEL_DEFAULT_EPSR;  // xPSR (EPSR needs to set Thumb bit)
  sp--;
  *sp = (uint32_t)task->func;  // PC
  sp--;
  *sp = 0;  // LR
  sp -= 5;
  sp--;
  *sp = KERNEL_DEFAULT_EXCRETURN;  // EXC_RETURN to thread mode from SVCall handler for r14
  sp -= 8;                         // r11-r4
  task->sp = sp;
}

void task_init(task_t *task, task_func_t func, uint32_t priority, uint32_t *stack,
               size_t stack_size) {
  // sp grows down, and *stack is a pointer to the first element of the stack in contiguous
  // memory
  task->sp         = (stack + stack_size - 1);
  task->stack_size = stack_size;
  task->func       = func;
  task->priority   = priority;

  task->list_item.item = (void *)task;
  task->list_item.prev = NULL;
  task->list_item.next = NULL;

  task_init_stack(task);
}

void task_setready(task_t *task) {
  list_insert_end(&_ready_tasklists[task->priority], &task->list_item);
}

void kernel_set_basepri(uint32_t priority) {
  asm volatile("msr basepri, %0" ::"r"(priority) : "memory");
}

void kernel_init() {
  for (size_t i = 0; i < MAX_PRIORITIES; i++) list_init(&_ready_tasklists[i]);

  // Create the root idle task with lowest task priority
  task_init(&_root_task, _root_task_func, MAX_PRIORITIES - 1, _root_stack,
            KERNEL_DEFAULT_STACK_SIZE);
  task_setready(&_root_task);
  _curr_task = &_root_task;

  // Now disable all interrupts before our first task starts
  kernel_set_basepri(KERNEL_MAX_PRIORITY);

  // Set PendSV and SysTick with lowest interrupt priority, SVC is highest
  SCB->SHPR3 = (KERNEL_MIN_PRIORITY << SCB_SHPR3_SYSTICKSHIFT) |
               (KERNEL_MIN_PRIORITY << SCB_SHPR3_PENDSVSHIFT);

  // Setup SysTick
  systick_init(SYS_CLOCK / KERNEL_TICK_HZ);
  _systicks = 0;
}

void __attribute__((naked)) kernel_start() {
  asm volatile(
      "cpsie i                  \n"  // Enable interrupts
      "cpsie f                  \n"
      "isb                      \n"  // Wait cpsie to complete (needed for enabling)
      "svc 0                    \n"  // Escalate to handler mode for a cleaner transition to
                                     // unprivileged thread mode thru EXC_RETURN
  );
}

void __attribute__((naked)) _svc_handler() {
  asm volatile(
      "ldr r0, =_root_task      \n"  // =_root_task is pointer to _root_task
      "ldr r1, [r0]             \n"  // Get the sp (first word) in task_t
      "ldmia r1!, {r4-r11, r14} \n"  // Load r4-r11 and r14 we saved in root_task_init_stack
      "msr psp, r1              \n"  // Set the task psp for when we do an exception return
      "mov r0, #0               \n"
      "msr basepri, r0          \n"  // Re-enable interrupts by clearing mask
      "mov r0, #0x1             \n"  // Set task execution to unprivileged
      "msr control, r0          \n"
      "isb                      \n"
      "bx r14                   \n"  // Use special EXC_RETURN value to enter thread mode at psp
  );
}

void task_yield() {
  SCB->ICSR |= SCB_ICSR_PENDSVSET;
}

void kernel_pick_task() {
  for (size_t i = 0; i < MAX_PRIORITIES; i++) {
    list_item_t *item = list_next(&_ready_tasklists[i]);
    if (item != NULL) {
      task_t *next_task = (task_t *)item->item;
      _curr_task        = next_task;
      return;
    }
  }
}

void __attribute__((naked)) _pendsv_handler() {
  asm volatile(
      "mrs r0, psp              \n"  // Save the psp since push/pop refers to msp here
      "ldr r1, =_curr_task      \n"  // Get address of _curr_task
      "ldr r2, [r1]             \n"  // Get sp in _curr_task
      "stmdb r0!, {r4-r11, r14} \n"  // Save context from previous task to its stack with its psp
      "str r0, [r2]             \n"  // Update prev task sp

      "push {r1}                \n"  // Save addr of _curr_task
      "mov r0, %0               \n"
      "msr basepri, r0          \n"  // Disable interrupts
      "isb                      \n"
      "bl kernel_pick_task      \n"  // Pick the next task and update _curr_task

      "pop {r1}                 \n"
      "ldr r2, [r1]             \n"
      "ldr r3, [r2]             \n"  // Get psp of new _curr_task
      "ldmia r3!, {r4-r11, r14} \n"  // Load its context from its psp
      "msr psp, r3              \n"  // Update global psp
      "mov r0, #0               \n"
      "msr basepri, r0          \n"  // Enable interrupts
      "isb                      \n"
      "bx r14                   \n" ::"i"(KERNEL_MAX_PRIORITY));
}

void _systick_handler() {
  if (_systicks == _systick_next_preempt) {
    // Queue a PendSV
    SCB->ICSR |= SCB_ICSR_PENDSVSET;
    _systick_next_preempt = _systicks + KERNEL_PREEMPT_TICKS;
  }
  _systicks++;
}

