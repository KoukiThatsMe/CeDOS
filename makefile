.RECIPEPREFIX = >

CURRENT_DIR		= $(shell pwd)
BUILD_DIR		= $(CURRENT_DIR)/build

GCC_PREFIX		= $(HOME)/opt/cross/i686-elf-/bin/i686-elf-
GCC_OPTIONS		= -O0 -std=c++0x -Wno-write-strings -Wall -Wextra -fno-exceptions -fno-rtti -ffreestanding

# OBJ_FILES = $(wildcard obj/asm/*.o) $(wildcard obj/cpp/*.o)

.PHONY: build
build: $(BUILD_DIR)/base.img
> 

.PHONY: clear
clear:
> @rm $(BUILD_DIR)/*.* 2> /dev/null; true

.PHONY: run
run:
> ./run.sh

.PHONY: rebuild
rebuild:
> make clear
> make build

$(BUILD_DIR)/base.img: $(BUILD_DIR)/boot.o
> $(GCC_PREFIX)ld -T link.txt -Map=mapfile.txt -o $(BUILD_DIR)/base.img --oformat binary

$(BUILD_DIR)/boot.o: boot.s
> $(GCC_PREFIX)as -o $(BUILD_DIR)/boot.o boot.s