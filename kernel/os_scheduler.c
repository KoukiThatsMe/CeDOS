#include "os_scheduler.h"
#include "os_paging.h"
#include "driv_text.h"

/*!
 * Executes a task.
 */
ProcessID sched_exec(void) {
    void* page_dir = create_empty_page_dir();
    switch_page_dir(page_dir);

    map_range_to(page_dir, (VIRT_ADDR)0x00000000, (PHYS_ADDR)0x00000000, PAGE_ENTRY_COUNT, 0b000000000011);
    
    // will not work because lower memory not mapped
    text_write("i did it :)\n");
    while(1) {}

}