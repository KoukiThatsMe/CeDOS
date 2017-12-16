.RECIPEPREFIX = >

.PHONY: build
build: boot.s
> $(GCC_PREFIX)as -o $(BUILD_DIR)/boot.o boot.s

.PHONY: clear
clear:
> @rm build/*.* 2> /dev/null; true

.PHONY: rebuild
rebuild:
> $(MAKE) clear
> $(MAKE) build
