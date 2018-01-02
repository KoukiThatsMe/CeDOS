/*! \file
 * Provides structures and functions to interact with and modify 
 * page directories and page tables.
 */
#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

//! Number of entries within a page table/directory.
#define PAGE_ENTRY_COUNT (uint32_t)(1 << 10)

//! Size of a single page or page table/directory.
#define PAGE_SIZE (uint32_t)(1 << 12)

typedef void* PHYS_ADDR;
typedef void* VIRT_ADDR;

//! Represents a single page entry in a page table.
typedef uint32_t PAGE_DIR_ENTRY;

//! Represents a single page table entry in a page directory.
typedef uint32_t PAGE_TABLE_ENTRY;

/*!
 * Sets cr3 to new page directory and returns the old one.
 * \param page_dir Address of new page directory that is supposed to be used.
 * \return Address of old page directory.
 */
__attribute__((always_inline)) inline void* switch_page_dir(void* page_dir) {
    uint32_t res;
    __asm__ volatile (  "mov %%cr3, %0\n"
                        "mov %1, %%cr3" : 
                        "=a" (res) :
                        "Nd" (page_dir));
    return (void*)res;
}

/*!
 * Invalidates all pages, forces buffer reload.
 */
__attribute__((always_inline)) inline void inv_all_pages(void) {
    __asm__ volatile (  "mov %cr3, %eax\n"
                        "mov %eax, %cr3");
}

int map_range_to(PHYS_ADDR page_dir, VIRT_ADDR dest, PHYS_ADDR src, uint32_t page_count, uint32_t flags);

/*!
 * Uses a free page to generate a new page directory which maps its last entry to itself.
 * \return Address of new page directory.
 */
PHYS_ADDR create_empty_page_dir(void);

#endif