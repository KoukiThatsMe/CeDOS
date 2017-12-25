.RECIPEPREFIX = >

.PHONY: build
build: boot.s
> $(GCC_PREFIX)as -o $(GLOBAL_BUILD)/boot.o boot.s

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build
