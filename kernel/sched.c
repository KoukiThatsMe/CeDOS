#include "cedos/sched.h"
#include "cedos/process.h"
#include "cedos/mm/paging.h"
#include "cedos/drivers/console.h"
#include "cedos/core.h"
#include "cedos/interrupts.h"
#include "cedos/pit.h"
#include "cedos/pic.h"
#include "assembly.h"
#include "common.h"
#include "cedos/drivers/speaker.h"

void* const SCHED_STACK = (void*)(0xC0400000);

PROCESS* get_slot(void) {
    static PROCESS free_slots[4];
    static uint32_t index = 0;
    return &(free_slots[index++]);
}

int sched_dispatcher(void);

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(PHYS_ADDR page_dir, VIRT_ADDR eip, uint32_t eflags, VIRT_ADDR ebp) {
    PHYS_ADDR tmp_page_dir = switch_page_dir(page_dir);
    PROCESS* p = get_slot();
    
    // set process context
    p->page_dir = page_dir;
    p->eip = sched_dispatcher;
    p->ebp = ebp;
    p->esp = ebp - sizeof(SCHED_FRAME);
    p->eflags = eflags;
    p->entry = (PROCESS_MAIN*)eip;

    // setup stack
    SCHED_FRAME* frame = (SCHED_FRAME*)(p->esp);
    frame->eax = frame->ebx = frame->ecx = frame->edx = 0;
    frame->esi = frame->edi = 0;
    frame->ebp = p->ebp;
    frame->esp = p->esp;
    frame->eip = sched_dispatcher;
    frame->cs = 0x8;
    frame->eflags = eflags;

    // TODO: add file descriptors for stdin, stdout and stderr

    p->state = PSTATE_READY;

    switch_page_dir(tmp_page_dir);

    return add_process(p);
}

PROCESS_ID current_pid;

void sched_interrupt_c(SCHED_FRAME * volatile frame, uint32_t volatile ebp) {
    //kpanic("SCHEDULER STACK INFO");
    PROCESS* process = get_process(current_pid);

    if (current_pid != 0) {
        process->esp = (uint32_t)frame;
        process->ebp = ebp;
        process->eip = frame->eip;
        process->eflags = frame->eflags;
    }

    // select next process
    static int pid = 0;
    pid = (pid + 1) % 4;
    current_pid = pid;

    printk("\n### SCHEDULER: SWITCH TO TASK %i\n", pid);

    // prepare to return to process
    switch_page_dir(process->page_dir);
    process = get_process(current_pid);
    frame = (volatile SCHED_FRAME*)(process->esp);
    ebp = process->ebp;
    frame->eip = process->eip;
    frame->eflags = process->eflags;

    // reset the timer
    pit_setup_channel(PIT_CHANNEL_0, PIT_MODE_0, 0xFFFF);

    pic1_eoi();

    //if (current_pid == 2) outb(0xFE, 0x64);
}

void idle(void) {
    printk(".");
    while (1) { }
}

int sched_init(void) {
    // create idle process
    sched_exec(create_empty_page_dir(), idle, eflags() | (1 << 9), 0xC0200000);

    return 1;
}

int sched_start(void) {
    current_pid = get_process(0);

    // perform the first timer interrupt manually
    pic_unmask_interrupt(0);
    INT(0x20);
}

void sched_yield(void) {
    INT(0x20);
}

int sched_kill(PROCESS_ID pid) {
    if (pid == current_pid) {
        //remove_process(pid);
        sched_yield();
        return 1;
    } else {
        //remove_process(pid);
        return 1;
    }
}

int sched_dispatcher(void) {
    printk("Dispatching process %i...\n", current_pid);

    PROCESS* this = get_process(current_pid);
    PROCESS_MAIN* entry = this->entry;

    // enter the actual program
    entry(0, (void*)0);

    printk("Process %i terminated.\n", current_pid);

    sched_kill(current_pid);
}