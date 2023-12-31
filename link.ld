ENTRY(_reset_handler)

MEMORY {
  FLASH (r)  : ORIGIN =  0x8000000, LENGTH = 1024K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 320K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
  /* ISR vector table comes first in flash since VTOR = 0 by default, and must be 4-byte aligned */
  .isr_vector : {
    . = ALIGN(4);
    KEEP (*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  /* code goes into flash */
  .text : {
    . = ALIGN(4);
    *(.text)         /* code including separated functions */
    *(.text*)
    *(.glue_7)       /* glue code for arm/thumb */
    *(.glue_7t)
    KEEP (*(.init))  /* C runtime init sections */
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  /* constant data goes into flash */
  .rodata : {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)      /* separated data */
    . = ALIGN(4);
  } > FLASH

  /* more C runtime init stuff */
  .preinit_array : {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > FLASH
  .init_array : {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > FLASH
  .fini_array : {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > FLASH

  /* initialized data goes into RAM at VMA after flash */
  _sidata = LOADADDR(.data);
  .data : {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT> FLASH

  /* uninitialized data goes into RAM too */
  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)        /* un-externed stuff */
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  ._user_heap_stack : {
    . = ALIGN(8);
    PROVIDE (end = .);  /* C runtime addresses for _sbrk */
    PROVIDE (_end = .);
    . = ALIGN(8);
  } > RAM

  /* get rid of unused sections from standard libraries */
  /DISCARD/ : {
    libc.a (*)
    libm.a (*)
    libgcc.a (*)
  }

  /* ARM metadata */
  .ARM.attributes 0 : {
    *(.ARM.attributes)
  }
}
