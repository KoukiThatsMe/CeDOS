.RECIPEPREFIX = >

export CURRENT_DIR		= $(shell pwd)
export BUILD_DIR		= $(CURRENT_DIR)/build
export INCLUDE_DIR		= $(CURRENT_DIR)/include
export DEBUG_DIR		= $(CURRENT_DIR)/debug

export GCC_PREFIX		= $(HOME)/opt/cross/i686-elf-/bin/i686-elf-
export GCC_OPTIONS		= -O0 -Wno-write-strings -Wall -Wextra -fno-exceptions -nostdlib -nostartfiles -ffreestanding

# OBJ_FILES = $(wildcard obj/asm/*.o) $(wildcard obj/cpp/*.o)

.PHONY: build
build:
> $(MAKE) -C boot build
> $(MAKE) -C second_stage build
> $(GCC_PREFIX)ld -T link.txt -Map=$(DEBUG_DIR)/mapfile.txt -o $(BUILD_DIR)/base.img --oformat binary

.PHONY: clear
clear:
> @rm $(BUILD_DIR)/*.* 2> /dev/null; true
> @$(MAKE) -C boot clear 2> /dev/null; true
> @$(MAKE) -C second_stage clear 2> /dev/null; true

.PHONY: run
run:
> ./run.sh

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build
