.RECIPEPREFIX = >

export CURRENT_DIR		= $(shell pwd)
export INCLUDE_DIR		= $(CURRENT_DIR)/include
export DEBUG_DIR		= $(CURRENT_DIR)/debug

export GCC_PREFIX		= $(HOME)/opt/cross/bin/i686-elf-

GLOBAL_BUILD		= $(CURRENT_DIR)/build
ifdef DEBUG
GCC_OPTIONS		= -D DEBUG -g -O0 -Wno-write-strings -Qn -Wall -Wextra -fno-exceptions -nostdlib -nostartfiles -ffreestanding -mgeneral-regs-only -mno-red-zone
LOCAL_BUILD = $(GLOBAL_BUILD)/debug
else
GCC_OPTIONS		= -O1 -Wno-write-strings -Qn -Wall -Wextra -fno-exceptions -nostdlib -nostartfiles -ffreestanding -mgeneral-regs-only -mno-red-zone
LOCAL_BUILD = $(GLOBAL_BUILD)/release
endif

export GCC_OPTIONS


.PHONY: build
build:
> @mkdir $(LOCAL_BUILD) 2> /dev/null; true
> $(MAKE) GLOBAL_BUILD=$(LOCAL_BUILD) -C boot build
> $(MAKE) GLOBAL_BUILD=$(LOCAL_BUILD) -C kernel build
> $(GCC_PREFIX)ld $(LOCAL_BUILD)/*.o -T link.txt -Map=$(DEBUG_DIR)/mapfile.txt -o $(GLOBAL_BUILD)/base.o
> $(GCC_PREFIX)objcopy --only-keep-debug $(GLOBAL_BUILD)/base.o $(DEBUG_DIR)/base.sym
> $(GCC_PREFIX)objcopy -O binary $(GLOBAL_BUILD)/base.o $(GLOBAL_BUILD)/base.img
> $(GCC_PREFIX)objdump -D $(GLOBAL_BUILD)/base.o > $(DEBUG_DIR)/objdump.txt


.PHONY: clear
clear:
> @rm -r $(CURRENT_DIR)/build/* 2> /dev/null; true

.PHONY: run
run:
> ./run.sh

.PHONY: debug
debug:
> $(MAKE) DEBUG=1 build

.PHONY: docs
docs:
> doxygen doxygen.cfg