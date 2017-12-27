#include "os_paging.h"
#include "linker.h"
#include "os_page_allocator.h"

//#define MAKE_PAGE_ENTRY(addr, flags) (uint32_t)(((uint32_t)(addr) & 0xFFFFF000) | (flags))

#define PAGE_DIRECTORY (PAGE_DIR_ENTRY[PAGE_ENTRY_COUNT])
#define PAGE_TABLE (PAGE_TABLE_ENTRY[PAGE_ENTRY_COUNT])

#define PAGE_DIR_INDEX(addr) (((uint32_t)(addr) >> 22) % PAGE_ENTRY_COUNT)
#define PAGE_TABLE_INDEX(addr) (((uint32_t)(addr) >> 12) % PAGE_ENTRY_COUNT)

#define PAGE_TABLE_MAPPED_ADDR(index) ((PAGE_TABLE_ENTRY*)(PAGE_SIZE * PAGE_ENTRY_COUNT * (PAGE_ENTRY_COUNT - 1) + PAGE_SIZE * index))
#define PAGE_DIR_MAPPED_ADDR ((PAGE_DIR_ENTRY*)(PAGE_TABLE_MAPPED_ADDR(PAGE_ENTRY_COUNT - 1)))

/*
int map_page_to(void* page_addr, uint32_t dir_index, uint32_t table_index) {
    PAGE_DIR_ENTRY* page_dir = PAGE_DIR_MAPPED_ADDR;

    if (!(page_dir[dir_index].present)) {
        #warning implement this
        // acquire new page table
    }

    PAGE_TABLE_ENTRY* page_table = PAGE_TABLE_MAPPED_ADDR(dir_index);

    if (!(page_table[table_index].present)) {
        #warning implement this
        // map page
    } else {
        // didn't works
        return -1;
    }
}

void* map_first_free(void* page_dir, void* search_start, void* page) {
    uint32_t table_start_i = PAGE_TABLE_INDEX(search_start);

    for (uint32_t dir_i = PAGE_DIR_INDEX(search_start); dir_i < (PAGE_ENTRY_COUNT - 1); dir_i++) {
        for (uint32_t table_i = table_start_i; table_i < (PAGE_ENTRY_COUNT - 1); table_i++) {
            if (map_page_to(page, ))
        }

        table_start_i = 0;
    }

    return 0;
}

void* create_empty_page_dir(void) {
    return 0;
    #warning IMPLEMENT STH.
}
*/