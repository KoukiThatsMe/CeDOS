.RECIPEPREFIX = >

S_OBJECTS = $(patsubst %.s,$(LOCAL_BUILD)/%.s.o,$(wildcard *.s))
C_OBJECTS = $(patsubst %.c,$(LOCAL_BUILD)/%.c.o,$(wildcard *.c))
OBJECTS = $(S_OBJECTS) $(C_OBJECTS)

LOCAL_BUILD = $(GLOBAL_BUILD)/boot_stage2

SUBDIRS = $(wildcard */.)

.PHONY: build
build: folder $(SUBDIRS) $(OBJECTS)
> $(GCC_PREFIX)ld $(wildcard $(LOCAL_BUILD)/*.o) -r -T link.txt -o $(GLOBAL_BUILD)/boot_stage2.o --oformat elf32-i386

.PHONY: folder
folder:
> @mkdir $(LOCAL_BUILD) 2> /dev/null; true

.PHONY: $(SUBDIRS)
$(SUBDIRS):
> make -C $@ GLOBAL_BUILD=$(LOCAL_BUILD) build

$(LOCAL_BUILD)/%.s.o: %.s
> $(GCC_PREFIX)as -o $@ $<

$(LOCAL_BUILD)/%.c.o: %.c $(wildcard *.h)
> $(GCC_PREFIX)gcc -c --prefix=$(GCC_PREFIX) $(GCC_OPTIONS) -o $@ $<