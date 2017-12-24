#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

void* get_free_page();
void mark_as_free(void* page_addr);

#endif