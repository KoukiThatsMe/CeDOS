.RECIPEPREFIX = >

.PHONY: build
build: build/entry.o
> $(GCC_PREFIX)ld -r -T link.txt -o $(BUILD_DIR)/kernel.o --oformat elf32-i386

.PHONY: clear
clear:
> @rm build/*.* 2> /dev/null; true

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build

build/entry.o: entry.s
> $(GCC_PREFIX)as -o $@ $<
