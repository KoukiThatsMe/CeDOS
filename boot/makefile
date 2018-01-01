.RECIPEPREFIX = >

S_OBJECTS = $(patsubst %.s,$(LOCAL_BUILD)/%.s.o,$(wildcard *.s))
C_OBJECTS = $(patsubst %.c,$(LOCAL_BUILD)/%.c.o,$(wildcard *.c))
OBJECTS = $(S_OBJECTS) $(C_OBJECTS)

LOCAL_BUILD = $(GLOBAL_BUILD)

SUBDIRS = $(wildcard */.)

.PHONY: build
build: $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
> make -C $@ GLOBAL_BUILD=$(LOCAL_BUILD) build