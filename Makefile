# Tools
PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
AS = $(PREFIX)-as
OBJCOPY = $(PREFIX)-objcopy
GDB = $(PREFIX)-gdb

# Directories
CUBE = STM32CubeL4
HAL = $(CUBE)/Drivers/STM32L4xx_HAL_Driver
CMSIS = $(CUBE)/Drivers/CMSIS
CMSIS_DEV=$(CMSIS)/Device/ST/STM32L4xx
BSP = $(CUBE)/Drivers/BSP

# Include path
INC =  -I$(HAL)/Inc
INC += -I$(CMSIS)/Include
INC += -I$(CMSIS_DEV)/Include
INC += -Ihal

# Flags
CFLAGS  = -std=c17 -Wall -Wextra -Werror -g
CFLAGS += -mcpu=cortex-m4 -mthumb -mlittle-endian -DSTM32L476xx
CFLAGS += -T $(LINKER_SCRIPT) --specs=nosys.specs

# CFLAGS for just our sources
EXTRA_CFLAGS = -pedantic

# Startup Files
STARTUP_S = $(CMSIS_DEV)/Source/Templates/gcc/startup_stm32l476xx.s
SYSTEM_C = $(CMSIS_DEV)/Source/Templates/system_stm32l4xx.c
STARTUP_O = build/startup/$(notdir $(STARTUP_S:.s=.o))
SYSTEM_O = build/startup/$(notdir $(SYSTEM_C:.c=.o))
LINKER_SCRIPT = src/STM32L476VGTx_FLASH.ld

# Our files
SRCS = $(wildcard src/*.c)
OBJS = $(addprefix build/,$(SRCS:.c=.o))

HAL_SRCS = stm32l4xx_hal.c \
           stm32l4xx_hal_usart.c \
           stm32l4xx_hal_gpio.c \
           stm32l4xx_hal_dma.c \
           stm32l4xx_hal_cortex.c \
           stm32l4xx_hal_rcc.c \
           stm32l4xx_hal_rcc_ex.c \
           stm32l4xx_hal_pwr.c \
           stm32l4xx_hal_pwr_ex.c

HAL_OBJS=$(addprefix build/hal/,$(HAL_SRCS:.c=.o))

BSP_SRCS=
BSP_OBJS=$(addprefix build/bsp/,$(BSP_SRCS:.c=.o))

# Recipes

.PHONY: all
all: build/lmeter.bin

.PHONY: clean
clean:
	rm -f $(OBJS) \
	    $(STARUP_O) \
	    $(SYSTEM_O) \
	    $(HAL_OBJS) \
	    $(BSP_OBS) \
	    build/meter.elf build/meter.bin

# directories
build/src:
	mkdir -p build/src
build/hal:
	mkdir -p build/hal
build/bsp:
	mkdir -p build/bsp
build/startup:
	mkdir -p build/startup

build/src/%.o: src/%.c build/src
	$(CC) -c $(INC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(STARTUP_O): $(STARTUP_S) build/startup
	$(AS) -c $(INC) $< -o $@

$(SYSTEM_O): $(SYSTEM_C) build/startup
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/hal/%.o: $(HAL)/Src/%.c build/hal
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/bsp/%.o: $(BSP)/STM32L476G-Discovery/%.c build/bsp
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/lmeter.elf: $(OBJS) $(HAL_OBJS) $(BSP_OBJS) $(STARTUP_O) $(SYSTEM_O)
	$(CC) -o $@ $(CFLAGS) $^

build/lmeter.bin: build/lmeter.elf
	$(OBJCOPY) -O binary $< $@
