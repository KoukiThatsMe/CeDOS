.RECIPEPREFIX = >

S_OBJECTS = $(patsubst %.s,$(LOCAL_BUILD)/%.s.o,$(wildcard *.s))
C_OBJECTS = $(patsubst %.c,$(LOCAL_BUILD)/%.c.o,$(wildcard *.c))
OBJECTS = $(S_OBJECTS) $(C_OBJECTS)

.PHONY: build
build: $(OBJECTS)
> $(GCC_PREFIX)ld $^ -r -T link.txt -o $(GLOBAL_BUILD)/second_stage.o --oformat elf32-i386

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build

$(LOCAL_BUILD)/%.s.o: %.s
> $(GCC_PREFIX)as -o $@ $<

$(LOCAL_BUILD)/%.c.o: %.c $(wildcard *.h)
> $(GCC_PREFIX)gcc -c -I$(INCLUDE_DIR) --prefix=$(GCC_PREFIX) $(GCC_OPTIONS) -o $@ $<