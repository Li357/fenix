TARGET = fenix

DEBUG = 1

BUILD_DIR = build

C_SOURCES = drivers/gpio.c drivers/usart.c drivers/nvic.c drivers/ethernet.c drivers/systick.c drivers/rcc.c src/syscall.c src/main.c src/kernel.c src/list.c src/net.c src/socket.c
C_INCLUDES = -Idrivers/include -Isrc/include
ASM_SOURCES = bootloader.s
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPU = -mcpu=cortex-m7
FPU = -mfpu=fpv5-d16 -mfloat-abi=hard
MCU = -mthumb $(CPU) $(FPU)

COMMONFLAGS = -Wall -Werror -fdata-sections -ffunction-sections -Og
ASFLAGS = $(MCU) $(COMMONFLAGS)
CFLAGS += $(MCU) $(C_INCLUDES) $(COMMONFLAGS)

ifeq ($(DEBUG), 1)
	CFLAGS += -g3 -gdwarf-5 -DDEBUG
else
	CFLAGS += -O3
endif

LDSCRIPT = link.ld

LIBS = -lc -lm -lnosys
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--no-warn-rwx-segments

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -Rf $(BUILD_DIR)
