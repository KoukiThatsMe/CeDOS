.RECIPEPREFIX = >

export CURRENT_DIR		= $(shell pwd)
export INCLUDE_DIR		= $(CURRENT_DIR)/include
export DEBUG_DIR		= $(CURRENT_DIR)/debug

export GCC_PREFIX		= $(HOME)/opt/cross/bin/i686-elf-

ifdef DEBUG
GLOBAL_BUILD		= $(CURRENT_DIR)/build/debug
GCC_OPTIONS		= -D DEBUG -g -O0 -Wno-write-strings -Qn -Wall -Wextra -fno-exceptions -nostdlib -nostartfiles -ffreestanding -mgeneral-regs-only
else
GLOBAL_BUILD		= $(CURRENT_DIR)/build/release
GCC_OPTIONS		= -O1 -Wno-write-strings -Qn -Wall -Wextra -fno-exceptions -nostdlib -nostartfiles -ffreestanding -mgeneral-regs-only
endif

export GLOBAL_BUILD
export GCC_OPTIONS

export LOCAL_BUILD = $(GLOBAL_BUILD)

.PHONY: build
build:
> @mkdir $(GLOBAL_BUILD) 2> /dev/null; true
> @mkdir $(GLOBAL_BUILD)/boot 2> /dev/null; true
> @mkdir $(GLOBAL_BUILD)/second_stage 2> /dev/null; true
> @mkdir $(GLOBAL_BUILD)/kernel 2> /dev/null; true
> $(MAKE) LOCAL_BUILD=$(GLOBAL_BUILD)/boot -C boot build
> $(MAKE) LOCAL_BUILD=$(GLOBAL_BUILD)/second_stage -C second_stage build
> $(MAKE) LOCAL_BUILD=$(GLOBAL_BUILD)/kernel -C kernel build
> $(GCC_PREFIX)ld $(wildcard $(GLOBAL_BUILD)/*.o) -T link.txt -Map=$(DEBUG_DIR)/mapfile.txt -o build/base.o
> $(GCC_PREFIX)objcopy --only-keep-debug build/base.o $(DEBUG_DIR)/base.sym
> $(GCC_PREFIX)objcopy -O binary build/base.o build/base.img

.PHONY: clear
clear:
> @rm -r $(CURRENT_DIR)/build/* 2> /dev/null; true

.PHONY: run
run:
> ./run.sh

.PHONY: rebuild
rebuild:
> $(MAKE) $(MAKEFLAGS) clear
> $(MAKE) $(MAKEFLAGS) build

.PHONY: objdump
objdump:
> $(GCC_PREFIX)objdump -D $(FILE) > debug/objdump.txt

.PHONY: debug
debug:
> $(MAKE) DEBUG=1 build

.PHONY: docs
docs:
> doxygen doxygen.cfg