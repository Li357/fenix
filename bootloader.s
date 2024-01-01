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
  .word _wwdg_handler
  .word _pvd_handler
  .word _tampstamp_handler
  .word _rtcwake_handler
  .word _flash_handler
  .word _rcc_handler
  .word _exti0_handler
  .word _exti1_handler
  .word _exti2_handler
  .word _exti3_handler
  .word _exti4_handler
  .word _dma1s0_handler
  .word _dma1s1_handler
  .word _dma1s2_handler
  .word _dma1s3_handler
  .word _dma1s4_handler
  .word _dma1s5_handler
  .word _dma1s6_handler
  .word _adc_handler
  .word _can1tx_handler
  .word _can1rx0_handler
  .word _can1rx1_handler
  .word _can1sce_handler
  .word _exti95_handler
  .word _tim1brk_tim9_handler
  .word _tim1up_tim10_handler
  .word _tim1trgcom_tim11_handler
  .word _tim1cc_handler
  .word _tim2_handler
  .word _tim3_handler
  .word _tim4_handler
  .word _i2c1ev_handler
  .word _i2c1er_handler
  .word _i2c2ev_handler
  .word _i2c2er_handler
  .word _spi1_handler
  .word _spi2_handler
  .word _usart1_handler
  .word _usart2_handler
  .word _usart3_handler
  .word _exti15_10_handler
  .word _rtcalarm_handler
  .word _otgfswake_handler
  .word _tim8brk_tim12_handler
  .word _tim8up_tim13_handler
  .word _tim8trgcom_tim14_handler
  .word _tim8cc_handler
  .word _dma1s7_handler
  .word _fsmc_handler
  .word _sdmmc1_handler
  .word _tim5_handler
  .word _spi3_handler
  .word _uart4_handler
  .word _uart5_handler
  .word _tim6_dac_handler
  .word _tim7_handler
  .word _dma2s0_handler
  .word _dma2s1_handler
  .word _dma2s2_handler
  .word _dma2s3_handler
  .word _dma2s4_handler
  .word _eth_handler
  .word _ethwake_handler
  .word _can2tx_handler
  .word _can2rx0_handler
  .word _can2rx1_handler
  .word _can2sce_handler
  .word _otgfs_handler
  .word _dma2s5_handler
  .word _dma2s6_handler
  .word _dma2s7_handler
  .word _usart6_handler
  .word _i2c3ev_handler
  .word _i2c3er_handler
  .word _otghsep1out_handler
  .word _otghsep1in_handler
  .word _otghswake_handler
  .word _otghs_handler
  .word _dcmi_handler
  .word _cryp_handler
  .word _hashrng_handler
  .word _fpu_handler
  .word _uart7_hander
  .word _uart8_handler
  .word _spi4_handler
  .word _spi5_handler
  .word _spi6_handler
  .word _sai1_handler
  .word _lcdtft_handler
  .word _lcdtfterr_handler
  .word _dma2d_handler
  .word _sai2_handler
  .word _quadspi_handler
  .word _lptimer1_handler
  .word _hdmicec_handler
  .word _i2c4ev_handler
  .word _i2c4er_handler
  .word _spdifrx_handler

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
  .weak _wwdg_handler
  .thumb_set _wwdg_handler, _default_handler
  .weak _pvd_handler
  .thumb_set _pvd_handler, _default_handler
  .weak _tampstamp_handler
  .thumb_set _tampstamp_handler, _default_handler
  .weak _rtcwake_handler
  .thumb_set _rtcwake_handler, _default_handler
  .weak _flash_handler
  .thumb_set _flash_handler, _default_handler
  .weak _rcc_handler
  .thumb_set _rcc_handler, _default_handler
  .weak _exti0_handler
  .thumb_set _exti0_handler, _default_handler
  .weak _exti1_handler
  .thumb_set _exti1_handler, _default_handler
  .weak _exti2_handler
  .thumb_set _exti2_handler, _default_handler
  .weak _exti3_handler
  .thumb_set _exti3_handler, _default_handler
  .weak _exti4_handler
  .thumb_set _exti4_handler, _default_handler
  .weak _dma1s0_handler
  .thumb_set _dma1s0_handler, _default_handler
  .weak _dma1s1_handler
  .thumb_set _dma1s1_handler, _default_handler
  .weak _dma1s2_handler
  .thumb_set _dma1s2_handler, _default_handler
  .weak _dma1s3_handler
  .thumb_set _dma1s3_handler, _default_handler
  .weak _dma1s4_handler
  .thumb_set _dma1s4_handler, _default_handler
  .weak _dma1s5_handler
  .thumb_set _dma1s5_handler, _default_handler
  .weak _dma1s6_handler
  .thumb_set _dma1s6_handler, _default_handler
  .weak _adc_handler
  .thumb_set _adc_handler, _default_handler
  .weak _can1tx_handler
  .thumb_set _can1tx_handler, _default_handler
  .weak _can1rx0_handler
  .thumb_set _can1rx0_handler, _default_handler
  .weak _can1rx1_handler
  .thumb_set _can1rx1_handler, _default_handler
  .weak _can1sce_handler
  .thumb_set _can1sce_handler, _default_handler
  .weak _exti95_handler
  .thumb_set _exti95_handler, _default_handler
  .weak _tim1brk_tim9_handler
  .thumb_set _tim1brk_tim9_handler, _default_handler
  .weak _tim1up_tim10_handler
  .thumb_set _tim1up_tim10_handler, _default_handler
  .weak _tim1trgcom_tim11_handler
  .thumb_set _tim1trgcom_tim11_handler, _default_handler
  .weak _tim1cc_handler
  .thumb_set _tim1cc_handler, _default_handler
  .weak _tim2_handler
  .thumb_set _tim2_handler, _default_handler
  .weak _tim3_handler
  .thumb_set _tim3_handler, _default_handler
  .weak _tim4_handler
  .thumb_set _tim4_handler, _default_handler
  .weak _i2c1ev_handler
  .thumb_set _i2c1ev_handler, _default_handler
  .weak _i2c1er_handler
  .thumb_set _i2c1er_handler, _default_handler
  .weak _i2c2ev_handler
  .thumb_set _i2c2ev_handler, _default_handler
  .weak _i2c2er_handler
  .thumb_set _i2c2er_handler, _default_handler
  .weak _spi1_handler
  .thumb_set _spi1_handler, _default_handler
  .weak _spi2_handler
  .thumb_set _spi2_handler, _default_handler
  .weak _usart1_handler
  .thumb_set _usart1_handler, _default_handler
  .weak _usart2_handler
  .thumb_set _usart2_handler, _default_handler
  .weak _usart3_handler
  .thumb_set _usart3_handler, _default_handler
  .weak _exti15_10_handler
  .thumb_set _exti15_10_handler, _default_handler
  .weak _rtcalarm_handler
  .thumb_set _rtcalarm_handler, _default_handler
  .weak _otgfswake_handler
  .thumb_set _otgfswake_handler, _default_handler
  .weak _tim8brk_tim12_handler
  .thumb_set _tim8brk_tim12_handler, _default_handler
  .weak _tim8up_tim13_handler
  .thumb_set _tim8up_tim13_handler, _default_handler
  .weak _tim8trgcom_tim14_handler
  .thumb_set _tim8trgcom_tim14_handler, _default_handler
  .weak _tim8cc_handler
  .thumb_set _tim8cc_handler, _default_handler
  .weak _dma1s7_handler
  .thumb_set _dma1s7_handler, _default_handler
  .weak _fsmc_handler
  .thumb_set _fsmc_handler, _default_handler
  .weak _sdmmc1_handler
  .thumb_set _sdmmc1_handler, _default_handler
  .weak _tim5_handler
  .thumb_set _tim5_handler, _default_handler
  .weak _spi3_handler
  .thumb_set _spi3_handler, _default_handler
  .weak _uart4_handler
  .thumb_set _uart4_handler, _default_handler
  .weak _uart5_handler
  .thumb_set _uart5_handler, _default_handler
  .weak _tim6_dac_handler
  .thumb_set _tim6_dac_handler, _default_handler
  .weak _tim7_handler
  .thumb_set _tim7_handler, _default_handler
  .weak _dma2s0_handler
  .thumb_set _dma2s0_handler, _default_handler
  .weak _dma2s1_handler
  .thumb_set _dma2s1_handler, _default_handler
  .weak _dma2s2_handler
  .thumb_set _dma2s2_handler, _default_handler
  .weak _dma2s3_handler
  .thumb_set _dma2s3_handler, _default_handler
  .weak _dma2s4_handler
  .thumb_set _dma2s4_handler, _default_handler
  .weak _eth_handler
  .thumb_set _eth_handler, _default_handler
  .weak _ethwake_handler
  .thumb_set _ethwake_handler, _default_handler
  .weak _can2tx_handler
  .thumb_set _can2tx_handler, _default_handler
  .weak _can2rx0_handler
  .thumb_set _can2rx0_handler, _default_handler
  .weak _can2rx1_handler
  .thumb_set _can2rx1_handler, _default_handler
  .weak _can2sce_handler
  .thumb_set _can2sce_handler, _default_handler
  .weak _otgfs_handler
  .thumb_set _otgfs_handler, _default_handler
  .weak _dma2s5_handler
  .thumb_set _dma2s5_handler, _default_handler
  .weak _dma2s6_handler
  .thumb_set _dma2s6_handler, _default_handler
  .weak _dma2s7_handler
  .thumb_set _dma2s7_handler, _default_handler
  .weak _usart6_handler
  .thumb_set _usart6_handler, _default_handler
  .weak _i2c3ev_handler
  .thumb_set _i2c3ev_handler, _default_handler
  .weak _i2c3er_handler
  .thumb_set _i2c3er_handler, _default_handler
  .weak _otghsep1out_handler
  .thumb_set _otghsep1out_handler, _default_handler
  .weak _otghsep1in_handler
  .thumb_set _otghsep1in_handler, _default_handler
  .weak _otghswake_handler
  .thumb_set _otghswake_handler, _default_handler
  .weak _otghs_handler
  .thumb_set _otghs_handler, _default_handler
  .weak _dcmi_handler
  .thumb_set _dcmi_handler, _default_handler
  .weak _cryp_handler
  .thumb_set _cryp_handler, _default_handler
  .weak _hashrng_handler
  .thumb_set _hashrng_handler, _default_handler
  .weak _fpu_handler
  .thumb_set _fpu_handler, _default_handler
  .weak _uart7_hander
  .thumb_set _uart7_hander, _default_handler
  .weak _uart8_handler
  .thumb_set _uart8_handler, _default_handler
  .weak _spi4_handler
  .thumb_set _spi4_handler, _default_handler
  .weak _spi5_handler
  .thumb_set _spi5_handler, _default_handler
  .weak _spi6_handler
  .thumb_set _spi6_handler, _default_handler
  .weak _sai1_handler
  .thumb_set _sai1_handler, _default_handler
  .weak _lcdtft_handler
  .thumb_set _lcdtft_handler, _default_handler
  .weak _lcdtfterr_handler
  .thumb_set _lcdtfterr_handler, _default_handler
  .weak _dma2d_handler
  .thumb_set _dma2d_handler, _default_handler
  .weak _sai2_handler
  .thumb_set _sai2_handler, _default_handler
  .weak _quadspi_handler
  .thumb_set _quadspi_handler, _default_handler
  .weak _lptimer1_handler
  .thumb_set _lptimer1_handler, _default_handler
  .weak _hdmicec_handler
  .thumb_set _hdmicec_handler, _default_handler
  .weak _i2c4ev_handler
  .thumb_set _i2c4ev_handler, _default_handler
  .weak _i2c4er_handler
  .thumb_set _i2c4er_handler, _default_handler
  .weak _spdifrx_handler
  .thumb_set _spdifrx_handler, _default_handler

