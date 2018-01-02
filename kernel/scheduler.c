#include "cedos/scheduler.h"
#include "cedos/process.h"
#include "cedos/mm/paging.h"
#include "cedos/drivers/console.h"
#include "cedos/kernel.h"

PROCESS* get_slot(void) {
    static PROCESS free_slots[4];
    static uint32_t index = 0;
    return &(free_slots[index++]);
}

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(PHYS_ADDR page_dir, VIRT_ADDR eip, VIRT_ADDR esp) {
    PROCESS* p = get_slot();
    
    p->page_dir = page_dir;
    p->eip = eip;
    p->esp = esp;

    // TODO: add file descriptors for stdin, stdout and stderr

    p->state = PSTATE_READY;

    return add_process(p);
}

int sched_init(void) {
    // TODO: create and start idle process
    return 1;
}