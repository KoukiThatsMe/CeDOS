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

//! Represents a single page entry in a page table.
typedef union {
    uint32_t entry;
    struct
    {
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_supervisor : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t __zero : 1;
        uint32_t page_size : 1;
        uint32_t __ignored : 1;
        uint32_t available : 3;
        uint32_t page_table_addr : 20;
    } __attribute__((packed)) fields;
} PAGE_DIR_ENTRY;

//! Represents a single page table entry in a page directory.
typedef union {
    uint32_t entry;
    uint8_t bytes[4];
    struct {
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_supervisor : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1;
        uint32_t __zero : 1;
        uint32_t global : 1;
        uint32_t available : 3;
        uint32_t page_addr : 20;
    } __attribute__((packed)) fields;
} PAGE_TABLE_ENTRY;

/*!
 * Sets cr3 to new page directory and returns the old one.
 * \param page_dir Address of new page directory that is supposed to be used.
 * \return Address of old page directory.
 */
inline void* switch_page_dir(void* page_dir) {
    uint32_t res;
    __asm__ volatile (  "mov %%cr3, %0\n"
                        "mov %1, %%cr3" : 
                        "=a" (res) :
                        "Nd" (page_dir));
    return res;
}

/*!
 * Uses a free page to generate a new page directory which maps its last entry to itself.
 * \return Address of new page directory.
 */
void* create_empty_page_dir(void);

#endif