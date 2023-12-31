/**
 * STM32F756 bootloader
 * - Set the initial SP interrupt vector table in flash (isr_vector section used in linker)
 * - Load this data from flash on startup, also zero out BSS sections
 * - Branch to C runtime init
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
  str r4, [r0, r3]
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

  /* then initialize clocks, C runtime and call main() */
  bl _system_init
  bl __libc_init_array
  bl main
  bx lr

.size _reset_handler, . - _reset_handler

/* default interrupt handler is just infinite loop */
_default_handler:
  b _default_handler
.size _default_handler, . - _default_handler

/**
 * set the initial SP and interrupt vector
 * reference: 2.4.4 of https://www.st.com/resource/en/programming_manual/pm0253-stm32f7-series-and-stm32h7-series-cortexm7-processor-programming-manual-stmicroelectronics.pdf
 */
.section .isr_vector,"a",%progbits
.type _isr_vector, %object
.size _isr_vector, . - _isr_vector
_isr_vector:
  .word _estack
  .word _reset_handler
  .word _nmi_handler
  .word _hardf_handler
  .word _memmanagef_handler
  .word _busf_handler
  .word _usagef_handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word _svc_handler
  .word _debug_handler
  .word 0
  .word _pendsv_handler
  .word _systick_handler

  /* peripheral interrupts IRQ0-239 */

  /* set weak aliases so these can be overrided in our application */
  .weak _nmi_handler
  .thumb_set _nmi_handler, _default_handler
  .weak _hardf_handler
  .thumb_set _hardf_handler, _default_handler
  .weak _memmanagef_handler
  .thumb_set _memmanagef_handler, _default_handler
  .weak _busf_handler
  .thumb_set _busf_handler, _default_handler
  .weak _usagef_handler
  .thumb_set _usagef_handler, _default_handler
  .weak _svc_handler
  .thumb_set _svc_handler, _default_handler
  .weak _debug_handler
  .thumb_set _debug_handler, _default_handler
  .weak _pendsv_handler
  .thumb_set _pendsv_handler, _default_handler
  .weak _systick_handler
  .thumb_set _systick_handler, _default_handler

