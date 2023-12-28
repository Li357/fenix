# Fenix

A bare-metal project on the STM32F756ZG Nucleo board for learning everything about MCUs from first principles, namely:
- ARM toolchain (compared to x86, newlib, different ABI)
- Linker scripts and basic ASM bootloader to setup RAM and call main()
- How the C runtime is initialized
- How to read datasheets
- How to write software drivers

The goal is to write drivers for USART, I2C, ADC/DAC, timers, maybe a custom VGA controller (and separate board with a connector for ok-quality video out, or driver for LCD-TFT controller), maybe a custom Ethernet driver. Then some kind of RTOS or simple context switching with priorities + IPC to manage all these drivers and events.
