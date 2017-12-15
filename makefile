.RECIPEPREFIX = >

export CURRENT_DIR		= $(shell pwd)
export BUILD_DIR		= $(CURRENT_DIR)/build

export GCC_PREFIX		= $(HOME)/opt/cross/i686-elf-/bin/i686-elf-
export GCC_OPTIONS		= -O0 -std=c++0x -Wno-write-strings -Wall -Wextra -fno-exceptions -fno-rtti -ffreestanding

# OBJ_FILES = $(wildcard obj/asm/*.o) $(wildcard obj/cpp/*.o)

.PHONY: build
build:
> $(MAKE) -C boot build
> $(MAKE) -C second_stage build
> $(GCC_PREFIX)ld -T link.txt -Map=mapfile.txt -o $(BUILD_DIR)/base.img --oformat binary

.PHONY: clear
clear:
> @rm $(BUILD_DIR)/*.* 2> /dev/null; true

.PHONY: run
run:
> ./run.sh

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build
