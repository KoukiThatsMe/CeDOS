#include "os_paging.h"
#include "linker.h"
#include "os_page_allocator.h"
#include "os_string.h"

#define MAKE_PAGE_ENTRY(addr, flags) (uint32_t)(((uint32_t)(addr) & 0xFFFFF000) | (flags))

#define PAGE_DIRECTORY (PAGE_DIR_ENTRY[PAGE_ENTRY_COUNT])
#define PAGE_TABLE (PAGE_TABLE_ENTRY[PAGE_ENTRY_COUNT])

#define PAGE_DIR_INDEX(addr) (((uint32_t)(addr) >> 22) % PAGE_ENTRY_COUNT)
#define PAGE_TABLE_INDEX(addr) (((uint32_t)(addr) >> 12) % PAGE_ENTRY_COUNT)

#define PAGE_MAPPED_ADDR(dir_index, table_index) (void*)((dir_index * PAGE_ENTRY_COUNT + table_index) * PAGE_SIZE)

#define PAGE_TABLE_MAPPED_ADDR(index) \
        ((PAGE_TABLE_ENTRY*)(PAGE_MAPPED_ADDR((PAGE_ENTRY_COUNT - 1), index)))
#define PAGE_DIR_MAPPED_ADDR \
        ((PAGE_DIR_ENTRY*)(PAGE_TABLE_MAPPED_ADDR(PAGE_ENTRY_COUNT - 1)))

#define PAGE_TABLE_ALT_MAPPED_ADDR(index) \
        ((PAGE_TABLE_ENTRY*)(PAGE_MAPPED_ADDR((PAGE_ENTRY_COUNT - 2), index)))
#define PAGE_DIR_ALT_MAPPED_ADDR \
        ((PAGE_DIR_ENTRY*)(PAGE_TABLE_ALT_MAPPED_ADDR(PAGE_ENTRY_COUNT - 1)))

#define PAGE_TABLE_FLAGS 0b000000000011

/*!
* WARNING: This method assumes the target page directory has already been mounted
* to the second to last page directory entry
*/
inline int is_addr_available(uint32_t dir_index, uint32_t table_index) {
    PAGE_DIR_ENTRY* page_dir = PAGE_DIR_ALT_MAPPED_ADDR;
    PAGE_TABLE_ENTRY* page_table = PAGE_TABLE_ALT_MAPPED_ADDR(table_index);
    
    if(!(page_dir[dir_index].fields.present)) {
        return 1;
    } else if (!(page_table[table_index].fields.present)) {
        return 1;
    } else {
        return 0;
    }
}

/*!
* WARNING: This method assumes the target page directory has already been mounted
* to the second to last page directory entry
*/
int map_page_to(PHYS_ADDR page_addr, uint32_t dir_index, uint32_t table_index, uint32_t flags) {
    PAGE_DIR_ENTRY* page_dir = PAGE_DIR_ALT_MAPPED_ADDR;
    PAGE_TABLE_ENTRY* page_table = PAGE_TABLE_ALT_MAPPED_ADDR(dir_index);
    
    int tmp = 0;

    if (!(page_dir[dir_index].fields.present)) {
        // acquire new page table
        void *new_page_table = get_free_page();
        page_dir[dir_index].entry = MAKE_PAGE_ENTRY(new_page_table, PAGE_TABLE_FLAGS);
    }

    if (!(page_table[table_index].fields.present)) {
        // map page
        page_table[table_index].entry = MAKE_PAGE_ENTRY(page_addr, flags);
        return 1;
    } else {
        // didn't works
        return 0;
    }
}

int map_range_to(PHYS_ADDR page_dir, VIRT_ADDR dest, PHYS_ADDR src, uint32_t page_count, uint32_t flags) {
    mount_page_dir(page_dir);

    
    for (uint32_t i = 0; i < page_count; i++) {
        uint32_t dir_index = PAGE_DIR_INDEX(dest + i * PAGE_SIZE);
        uint32_t table_index = PAGE_TABLE_INDEX(dest + i * PAGE_SIZE);
        
        if (!(is_addr_available(dir_index, table_index))) { return 0; }
    }
    
    for (uint32_t i = 0; i < page_count; i++) {
        uint32_t dir_index = PAGE_DIR_INDEX(dest + i * PAGE_SIZE);
        uint32_t table_index = PAGE_TABLE_INDEX(dest + i * PAGE_SIZE);
        
        map_page_to(src + i * PAGE_SIZE, dir_index, table_index, flags);
    }
}

void mount_page_dir(PHYS_ADDR page_dir) {
    PAGE_DIR_ENTRY *cur_page_dir = PAGE_DIR_MAPPED_ADDR;
    cur_page_dir[PAGE_ENTRY_COUNT - 2].entry = MAKE_PAGE_ENTRY(page_dir, PAGE_TABLE_FLAGS);
}

VIRT_ADDR map_first_free(PHYS_ADDR page_dir, VIRT_ADDR search_start, PHYS_ADDR page, uint32_t flags) {
    mount_page_dir(page_dir);
    uint32_t table_start_i = PAGE_TABLE_INDEX(search_start);

    for (uint32_t dir_i = PAGE_DIR_INDEX(search_start); dir_i < (PAGE_ENTRY_COUNT - 2); dir_i++) {
        for (uint32_t table_i = table_start_i; table_i < PAGE_ENTRY_COUNT; table_i++) {
            if (map_page_to(page, dir_i, table_i, flags)) {
                return PAGE_MAPPED_ADDR(dir_i, table_i);
            }
        }

        table_start_i = 0;
    }

    return 0;
}

PHYS_ADDR create_empty_page_dir(void) {
    PHYS_ADDR page_dir_phys = get_free_page();
    mount_page_dir(page_dir_phys);
    PAGE_DIR_ENTRY *page_dir = (VIRT_ADDR)0xffffe000; //PAGE_MAPPED_ADDR(PAGE_ENTRY_COUNT - 1, PAGE_ENTRY_CPAGE_MAPPED_ADDR(PAGE_ENTRY_COUNT - 1, PAGE_ENTRY_COUNT - 2)OUNT - 2);
    inv_all_pages();

    memset(page_dir, 0, PAGE_SIZE);
    
    // map to itself
    page_dir[PAGE_ENTRY_COUNT - 1].entry = MAKE_PAGE_ENTRY(page_dir_phys, PAGE_TABLE_FLAGS);
    
    page_dir = PAGE_DIR_ALT_MAPPED_ADDR;
    
    // map kernel
    page_dir[PAGE_DIR_INDEX(0xc0000000)].entry = PAGE_DIR_MAPPED_ADDR[PAGE_DIR_INDEX(0xc0000000)].entry;

    return page_dir_phys;
}