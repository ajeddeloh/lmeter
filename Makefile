NAME=lmeter
-include $(DEPS)

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
CFLAGS  = -std=c17 -Wall -Wextra -g3 -gdwarf-2 -MMD -MP -O2
CFLAGS += -mcpu=cortex-m4 -mthumb -DSTM32L476xx -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -T $(LINKER_SCRIPT) --specs=nosys.specs
LFLAGS = -lm

# CFLAGS for just our sources
EXTRA_CFLAGS = -pedantic

# Startup Files
STARTUP_S = src/startup_stm32l476xx.s
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

DEPS = $(OBJS:.o=.d) $(HAL_OBJS:.o=.d) $(BSP_OBJS:.o=.d) $(SYSTEM_O:.o=.d)
# Recipes

.PHONY: all
all: build/$(NAME).bin

.PHONY: clean
clean:
	rm -f $(OBJS) \
	    $(STARTUP_O) \
	    $(SYSTEM_O) \
	    $(HAL_OBJS) \
	    $(BSP_OBS) \
	    $(DEPS) \
	    build/$(NAME).elf build/$(NAME).bin

build/src/%.o: src/%.c #build/src
	mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(STARTUP_O): $(STARTUP_S) #build/startup
	mkdir -p $(@D)
	$(AS) -c $(INC) $< -o $@

$(SYSTEM_O): $(SYSTEM_C) #build/startup
	mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/hal/%.o: $(HAL)/Src/%.c #build/hal
	mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/bsp/%.o: $(BSP)/STM32L476G-Discovery/%.c #build/bsp
	mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

build/$(NAME).elf: $(OBJS) $(HAL_OBJS) $(BSP_OBJS) $(STARTUP_O) $(SYSTEM_O)
	$(CC) -o $@ $(CFLAGS) $^ $(LFLAGS)

build/$(NAME).bin: build/$(NAME).elf
	$(OBJCOPY) -O binary $< $@

.PHONY: gdb
gdb: build/$(NAME).elf
	$(GDB) -ex 'target ext :3333' build/$(NAME).elf

.PHONY: flash
flash: build/$(NAME).elf
	$(GDB) -ex 'target ext :3333' -ex \
		'monitor program $(abspath build/$(NAME).elf)' \
		build/$(NAME).elf
