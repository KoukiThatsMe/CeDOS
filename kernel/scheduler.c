#include "cedos/scheduler.h"
#include "cedos/process.h"
#include "cedos/mm/paging.h"
#include "cedos/drivers/console.h"
#include "cedos/core.h"
#include "cedos/interrupts.h"
#include "cedos/pit.h"
#include "cedos/pic.h"
#include "assembly.h"
#include "cedos/drivers/speaker.h"

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

INTERRUPT(sched_interrupt, frame) {
    printk("--#");

    // reset the timer
    pit_setup_channel(PIT_CHANNEL_0, PIT_MODE_0, 0xFFFF);

    uint32_t big_ben[] = { 415, 330, 370, 247, 247, 370, 415, 330 };
    static int i = 0;

    bios_sp.make_sound(big_ben[(i / 8) % (sizeof(big_ben)/sizeof(big_ben[0]))], 1);
    i++;

    pic1_eoi();
}

int sched_init(void) {
    // TODO: create and start idle process

    //sched_exec(create_empty_page_dir(), (void*)0, (void*)0);

    // install timer interrupt
    install_interrupt(0x20, sched_interrupt, 0x08, INT_GATE);

    return 1;
}

int sched_start(void) {
    // perform the first timer interrupt manually
    INT(0x20);
}

int sched_dispatcher(void) {

}