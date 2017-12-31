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

export GCC_OPTIONS

LOCAL_BUILD = $(GLOBAL_BUILD)

.PHONY: build
build:
> @mkdir $(GLOBAL_BUILD) 2> /dev/null; true
> $(MAKE) GLOBAL_BUILD=$(LOCAL_BUILD) -C boot build
> $(MAKE) GLOBAL_BUILD=$(LOCAL_BUILD) -C second_stage build
> $(MAKE) GLOBAL_BUILD=$(LOCAL_BUILD) -C kernel build
> $(GCC_PREFIX)ld $(GLOBAL_BUILD)/*.o -T link.txt -Map=$(DEBUG_DIR)/mapfile.txt -o build/base.o
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