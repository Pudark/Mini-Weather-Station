# 工具链定义
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# MCU 配置
MCU = cortex-m3
TARGET = stm32f10x

# 编译选项
CFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=soft
CFLAGS += -Os -ffunction-sections -fdata-sections
CFLAGS += -Wall -Werror -std=c99
CFLAGS += -DUSE_STDPERIPH_DRIVER -D$(TARGET)

# 包含路径
INCLUDES = -Icore/inc
INCLUDES += -Idrivers/STM32F10x_FWLib/inc
INCLUDES += -Idrivers/hardware
INCLUDES += -Idrivers/system
INCLUDES += -I.

# 源文件
C_SOURCES = $(wildcard core/src/*.c)
C_SOURCES += $(wildcard drivers/STM32F10x_FWLib/src/*.c)
C_SOURCES += $(wildcard drivers/hardware/*.c)
C_SOURCES += $(wildcard drivers/system/*.c)
C_SOURCES += drivers/USER/main.c  # 你的主文件

# 启动文件
ASM_SOURCES = core/startup/startup_stm32f10x_md.s

# 输出目录
BUILD_DIR = build
OBJECTS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
OBJECTS := $(addprefix $(BUILD_DIR)/,$(notdir $(OBJECTS)))
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# 链接脚本
LDSCRIPT = core/ld/STM32F103C8Tx_FLASH.ld

# 链接选项
LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=soft
LDFLAGS += -specs=nano.specs -specs=nosys.specs
LDFLAGS += -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref
LDFLAGS += -Wl,--gc-sections

# 目标文件
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf
TARGET_BIN = $(BUILD_DIR)/$(TARGET).bin
TARGET_HEX = $(BUILD_DIR)/$(TARGET).hex

# 构建规则
all: $(TARGET_BIN) $(TARGET_HEX)

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex $< $@

$(TARGET_ELF): $(OBJECTS) | $(BUILD_DIR)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -x assembler-with-cpp $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(TARGET_BIN)
	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program $(TARGET_BIN) verify reset exit"

debug:
	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg

.PHONY: all clean flash debug