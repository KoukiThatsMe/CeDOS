.RECIPEPREFIX = >

S_OBJECTS = $(patsubst %.s,$(LOCAL_BUILD)/%.s.o,$(wildcard *.s))
C_OBJECTS = $(patsubst %.c,$(LOCAL_BUILD)/%.c.o,$(wildcard *.c))
OBJECTS = $(S_OBJECTS) $(C_OBJECTS)

LOCAL_BUILD = $(GLOBAL_BUILD)/drivers

SUBDIRS = $(wildcard */.)

.PHONY: build
build: folder $(SUBDIRS) $(OBJECTS)
> $(GCC_PREFIX)ld $(wildcard $(LOCAL_BUILD)/*.o) -r -o $(GLOBAL_BUILD)/drivers.o --oformat elf32-i386

.PHONY: folder
folder:
> @mkdir $(LOCAL_BUILD) 2> /dev/null; true

.PHONY: $(SUBDIRS)
$(SUBDIRS):
> make -C $@ GLOBAL_BUILD=$(LOCAL_BUILD) build

$(LOCAL_BUILD)/%.s.o: %.s
> $(GCC_PREFIX)as -o $@ $<

$(LOCAL_BUILD)/%.c.o: %.c $(wildcard *.h)
> $(GCC_PREFIX)gcc -c -I$(INCLUDE_DIR) --prefix=$(GCC_PREFIX) $(GCC_OPTIONS) -o $@ $<
