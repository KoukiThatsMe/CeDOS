.RECIPEPREFIX = >

.PHONY: build
build: build/entry.o build/gdt.o
> $(GCC_PREFIX)ld -T link.txt -o $(BUILD_DIR)/second_stage.o --oformat elf32-i386

.PHONY: clear
clear:
> @rm build/*.* 2> /dev/null; true

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build

build/entry.o: entry.s
> $(GCC_PREFIX)as -o $@ $<

build/gdt.o: gdt.c gdt.h
> $(GCC_PREFIX)gcc -c -I$(INCLUDE_DIR) --prefix=$(GCC_PREFIX) $(GCC_OPTIONS) -o $@ $<