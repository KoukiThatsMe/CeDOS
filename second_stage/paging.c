#include "paging.h"
#include "linker.h"

void *ss_memset(void *ptr, uint8_t value, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        ((uint8_t*)ptr)[i] = value;
    }
    return ptr;
}

void *create_kernel_environment() {
    PAGE_DIR_ENTRY (*pdir)[PAGE_ENTRY_COUNT] = (void*)(0 * PAGE_SIZE);
    PAGE_TABLE_ENTRY (*kernel)[PAGE_ENTRY_COUNT] = (void*)(1 * PAGE_SIZE);
    PAGE_TABLE_ENTRY (*lowmem)[PAGE_ENTRY_COUNT] = (void*)(2 * PAGE_SIZE);

    // set everything to zero
    ss_memset(pdir, 0, 3 * PAGE_SIZE);

    // map page directory to itself
    (*pdir)[PAGE_DIR_INDEX(PAGE_ENTRY_COUNT - 1)].entry = MAKE_PAGE_ENTRY(pdir, 0b000000000011);

    // map 4M of kernel to 0xC0000000
    (*pdir)[PAGE_DIR_INDEX(KERNEL_VMA)].entry = MAKE_PAGE_ENTRY(kernel, 0b000000000011);

    for (uint32_t i = 0; i < 1 << 10; i++) {
        (*kernel)[i].entry = MAKE_PAGE_ENTRY(0x100000 + PAGE_SIZE * i, 0b000000000011);
    }

    // identity map first 4M of memory
    (*pdir)[PAGE_DIR_INDEX(0)].entry = MAKE_PAGE_ENTRY(lowmem, 0b000000000011);

    for (uint32_t i = 0; i < 1 << 10; i++) {
        (*lowmem)[i].entry = MAKE_PAGE_ENTRY(PAGE_SIZE * i, 0b000000000011);
    }

    // map 

    return pdir;
}