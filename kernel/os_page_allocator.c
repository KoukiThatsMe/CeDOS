#include "os_page_allocator.h"
#include "os_paging.h"

uint8_t* first_free = (uint8_t*)0x00200000;

void* get_free_page() {
    void* res = first_free;
    first_free += PAGE_SIZE;
    return res;
}

void mark_as_free(void* page_addr) {
    
}