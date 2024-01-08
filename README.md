# Fenix

A bare-metal project on the STM32F756ZG Nucleo board for learning everything about MCUs from first principles, namely:
- ARM toolchain (compared to x86, newlib, different ABI)
- Linker scripts and basic ASM bootloader to setup RAM and call main()
- How the C runtime is initialized
- How to read datasheets
- How to write software drivers

Currently comes with:
- basic RTOS with context-switching between tasks inspired by FreeRTOS
- USART driver for logging
- Ethernet + ARP, ICMP support

Notes:
- Make sure you've got the ARM GCC toolchain (arm-none-eabi-) to build
- Emulate with Renode (>= v1.14.0.6117 to fix an annoying Ethernet bug)
- I'm working on macOS, so I needed the TUN/TAP kext (you can install Tunnelblick). There's also platform-specific bridging between `en0` and `tap0` to connect Renode to your host interface. The device names might be different for you - edit in `start.sh`

Build debug with `make` and then run Renode with `sudo ./start.sh`. Currently the board has a hardcoded IP address 192.168.0.254:

- ARP: `arping 192.168.0.254` -- you might need to `brew install arping` and add Homebrew's sbin to your path. The default MAC from Renode is `00:00:00:00:00:02`.
- ICMP: `ping 192.168.0.254`
