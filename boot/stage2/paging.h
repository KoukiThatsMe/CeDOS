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

#define MAKE_PAGE_ENTRY(addr, flags) (uint32_t)(((uint32_t)(addr) & 0xFFFFF000) | (flags))

#define PAGE_DIRECTORY (PAGE_DIR_ENTRY[PAGE_ENTRY_COUNT])
#define PAGE_TABLE (PAGE_TABLE_ENTRY[PAGE_ENTRY_COUNT])

#define PAGE_DIR_INDEX(addr) ((uint32_t)(addr) >> 22)
#define PAGE_TABLE_INDEX(addr) ((uint32_t)(addr) >> 12)

#endif