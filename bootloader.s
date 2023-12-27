/**
 * STM32F756 bootloader
 * - Set initial SP
 * - Set initial PC to the reset handler
 * - Set the interrupt vector table
 * - Branch to main()
 */

.syntax unified
.cpu cortex-m7
.fpu fpv5-d16

.global _isr_vector
.global _default_handler

/* linker script addresses for segments */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

.weak _reset_handler
.type _reset_handler, %function
_reset_handler:
  ldr sp, =_estack    /* copy SP from flash to RAM */

  /* copy all data from flash to RAM */  
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b _copy_data_init

  /* first copy initialized data */
_copy_data_loop:
  ldr r4, [r2, r3]    /* load from flash then store to RAM */
  str r4, [r0, r2]
  adds r3, r3, #4

_copy_data_init:
  adds r4, r0, r3
  cmp r4, r1
  bcc _copy_data_loop /* branch if r4 - r1 did borrow, i.e. r4 < r1 */

  /* then copy uninitialized data */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b _copy_bss_init

_copy_bss_loop:
  str r3, [r2]
  adds r2, r2, #4

_copy_bss_init:
  cmp r2, r4
  bcc _copy_bss_loop

  /* then initialize C runtime and call main() */
  bl __libc_init_array
  bl main

.size _reset_handler, . - _reset_handler

_default_handler:
  b _default_handler
.size _default_handler, . - _default_handler

/* set the initial SP and interrupt vector */
.section .isr_vector,"a",%progbits
.type _isr_vector, %object
.size _isr_vector, . - _isr_vector
_isr_vector:
  .word _estack
  .word _reset_handler 
