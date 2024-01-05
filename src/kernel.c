/*
 * A simple RTOS with preemptive context-switching (modeled after FreeRTOS)
 *
 * - Tasks are created with static stack size and given priority, and execute in
 *   thread mode
 * - Scheduler preempts in a number of ticks and always chooses the highest priority task
 *   at time of preemption
 *
 * - SVC is used to start the root idle task
 * - SysTick increments ticks and calls a PendSV if a task needs to be preempted
 * - PendSV performs the actual context switch to the next task
 * - PendSV and SysTick have lowest priority to allow handlers to run after tasks are ready
 *
 * main() initializes the root idle task and performs a SVC to start scheduler
 *  -> idle task is run
 *  -> SysTick occurs and increments tick count and checks if a context-switch is necessary
 *    -> Context switches to higher-priority task via a software PendSV
 */

#include "kernel.h"
#include <stddef.h>
#include "config.h"
#include "list.h"
#include "scb.h"
#include "stdio.h"
#include "systick.h"

void kernel_start_root_task();

static volatile uint32_t _systicks;
static list_t _ready_tasklists[MAX_PRIORITIES];
static task_t *_curr_task;

static task_t _root_task;
static uint32_t _root_stack[KERNEL_DEFAULT_STACK_SIZE];

static void _root_task_func() {
  while (1) {
    uint32_t until = _systicks + 1000;
    while (_systicks < until) {}
    puts("hi!\n");
  }
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
}

void root_task_init_stack() {
  _root_task.sp--;
  *_root_task.sp = 0x01000000;  // xPSR (EPSR needs to set Thumb bit)
  _root_task.sp--;
  *_root_task.sp = (uint32_t)_root_task_func;  // PC
  _root_task.sp--;
  // TODO: LR to error handler, shouldn't get here!
  _root_task.sp -= 5;  // r12, r3, r2, r1, r0
  _root_task.sp--;
  *_root_task.sp = 0xFFFFFFFD;  // EXC_RETURN to thread mode from SVCall handler for r14
  _root_task.sp -= 8;           // r11-r4
}

void task_setready(task_t *task) {
  list_insert_end(&_ready_tasklists[task->priority], &task->list_item);
}

void kernel_set_basepri(uint32_t priority) {
  asm volatile("msr basepri, %0" ::"r"(priority) : "memory");
}

void kernel_init(uint32_t tick_hz, uint32_t ticks_per_task) {
  for (size_t i = 0; i < MAX_PRIORITIES; i++) list_init(&_ready_tasklists[i]);

  // Create the root idle task with lowest task priority
  task_init(&_root_task, _root_task_func, MAX_PRIORITIES - 1, _root_stack,
            KERNEL_DEFAULT_STACK_SIZE);
  // Initialize the stack to "look" like a task that will be switched into
  root_task_init_stack();
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

  // Start the root task (should never return!)
  kernel_start_root_task();
}

void __attribute__((naked)) kernel_start_root_task() {
  asm volatile(
      "ldr r0, =_estack \n"  // Get the address of end of the stack
      "msr msp, r0      \n"  // Set main sp back to initial
      "cpsie i          \n"  // Enable interrupts
      "cpsie f          \n"
      "isb              \n"  // Wait for msr, cpsie to complete (needed for enabling)
      "dsb              \n"
      "svc 0            \n"  // Escalate to handler mode for a cleaner transition to unprivileged
                             // thread mode thru EXC_RETURN
  );
}

void __attribute__((naked)) _svc_handler() {
  asm volatile(
      "ldr r0, =_root_task      \n"  // =_root_task is pointer to _root_task
      "ldr r1, [r0]             \n"  // Get the sp (first word) in task_t
      "ldmia r1!, {r4-r11, r14} \n"  // Load r4-r11 and r14 we saved in root_task_init_stack
      "msr psp, r1              \n"  // Set the task psp for when we do an exception return
      "isb \n"
      "mov r0, #0               \n"
      "msr basepri, r0          \n"  // Re-enable interrupts by clearing mask
      "mov r0, #0x1             \n"  // Set task execution to unprivileged
      "msr control, r0          \n"
      "isb                      \n"
      "bx r14                   \n"  // Use special EXC_RETURN value to enter thread mode at psp
  );
}

void _systick_handler() {
  _systicks++;
}

