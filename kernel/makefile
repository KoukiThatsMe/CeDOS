.RECIPEPREFIX = >

S_OBJECTS = $(patsubst %.s,build/%.s.o,$(wildcard *.s))
C_OBJECTS = $(patsubst %.c,build/%.c.o,$(wildcard *.c))
OBJECTS = $(S_OBJECTS) $(C_OBJECTS)

.PHONY: build
build: $(OBJECTS)
> $(GCC_PREFIX)ld $^ -r -T link.txt -o $(BUILD_DIR)/kernel.o --oformat elf32-i386

.PHONY: clear
clear:
> @rm build/*.* 2> /dev/null; true

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build

build/%.s.o: %.s
> $(GCC_PREFIX)as -o $@ $<

build/%.c.o: %.c $(wildcard *.h)
> $(GCC_PREFIX)gcc -c -I$(INCLUDE_DIR) --prefix=$(GCC_PREFIX) $(GCC_OPTIONS) -o $@ $<
