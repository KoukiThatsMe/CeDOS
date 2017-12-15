.RECIPEPREFIX = >

.PHONY: build
build: build/entry.o
> $(GCC_PREFIX)ld -i build/entry.o -o $(BUILD_DIR)/second_stage.o --oformat elf32-i386

build/entry.o: entry.s
> $(GCC_PREFIX)as -o $@ $?