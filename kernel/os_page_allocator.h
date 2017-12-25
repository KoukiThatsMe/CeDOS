/*! \file
 * Keeps track of unused memory pages and provides functions to allocate or free them.
 */
#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

/*!
 * Returns a free page and marks it as used.
 * \return Address of the first byte on the page or NULL if no page was allocated.
 */
void* get_free_page();

/*!
 * Adds a previously allocated page as free.
 * \param page_addr Pointer to any byte within the page to be freed.
 */
void mark_as_free(void* page_addr);

#endif