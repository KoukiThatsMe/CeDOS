#include "cedos/sched/sched.h"
#include "cedos/sched/process.h"

#include "cedos/mm/paging.h"

#include "cedos/drivers/console.h"
#include "cedos/drivers/speaker.h"

#include "cedos/core.h"
#include "cedos/interrupts.h"
#include "cedos/pit.h"
#include "cedos/pic.h"

#include "assembly.h"
#include "common.h"

#define KERNEL_PRIVATE_STACK (void*)(0xC0600000)

#define PROCESS_STD_EFLAGS (0x00000286)

PROCESS* get_slot(void) {
    static PROCESS free_slots[4];
    static uint32_t index = 0;
    return &(free_slots[index++]);
}

int sched_dispatcher(void);

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(PHYS_ADDR page_dir, PROCESS_MAIN *entry) {
    // set process context
    PROCESS *p = get_slot();
    p->page_dir = page_dir;
    p->eip = sched_dispatcher;
    p->ebp = KERNEL_PRIVATE_STACK;
    p->esp = KERNEL_PRIVATE_STACK - sizeof(SCHED_FRAME);
    p->eflags = PROCESS_STD_EFLAGS;
    p->entry = entry;
    p->state = PSTATE_READY;

    // setup stack
    static SCHED_FRAME frame;
    frame.eax = frame.ebx = frame.ecx = frame.edx = 0;
    frame.esi = frame.edi = 0;
    frame.ebp = p->ebp;
    frame.esp = p->esp;
    frame.eflags = p->eflags;
    frame.eip = sched_dispatcher;
    frame.cs = 0x8;

    // load stack
    copy_to_pdir(&frame, sizeof(frame), p->page_dir, p->esp);

    // save stack checksum
    p->checksum = stack_check(&frame, &(&frame)[1]);

    PROCESS_ID pid = add_process(p);
    printk("Executing task %i...\n", pid);
    return pid;
}

PROCESS_ID current_pid;

void sched_interrupt_c(SCHED_FRAME * volatile frame, uint32_t volatile ebp) {
    //kpanic("SCHEDULER STACK INFO");
    PROCESS* current = get_process(current_pid);

    if (current_pid != 0) {
        current->esp = (uint32_t)frame;
        current->ebp = ebp;
        current->eip = frame->eip;
        current->eflags = frame->eflags;

        // save stack checksum
        current->checksum = stack_check(current->esp, current->ebp);
    }

    // select next process
    static int pid = 0;
    pid = pid % 3 + 1;
    current_pid = pid;

    //printk("\n### SCHEDULER: SWITCH TO TASK %i\n", pid);

    // prepare to return to process
    PROCESS* next = get_process(current_pid);
    switch_page_dir(next->page_dir);

    // check stack
    if (current_pid != 0 && next->checksum != stack_check(next->esp, next->ebp)) {
        printk("STACK DAMAGED: PROCESS %i, ESP %i, EBP %i\n", current_pid, next->esp, next->ebp);
        memdump((void*)(next->esp), (void*)(next->ebp - next->esp));
        kpanic("CRITICAL STACK DAMAGE");
    }

    // prepare stack
    frame = (volatile SCHED_FRAME*)(next->esp);
    ebp = next->ebp;
    //frame->cs = 0x08;
    //frame->eip = next->eip;
    //frame->eflags = next->eflags;
    frame->esp = next->esp;
    frame->ebp = next->ebp;


    // reset the timer
    pit_setup_channel(PIT_CHANNEL_0, PIT_MODE_0, 0xFFFF);

    pic1_eoi();

    //if (current_pid == 2) outb(0xFE, 0x64);
}

void idle(void) {
    printk(".");
    while (1) { }
}

extern void* sched_interrupt;

int sched_init(void) {
    // install scheduler interrupt
    install_interrupt(0x20, &sched_interrupt, 0x08, INT_GATE);

    // create idle process
    sched_exec(create_empty_page_dir(), idle);

    return 1;
}

int sched_start(void) {
    current_pid = 0;

    // perform the first timer interrupt manually
    pic_unmask_interrupt(0);
    INT(0x20);
}

void sched_yield(void) {
    INT(0x20);
}

int sched_kill(PROCESS_ID pid) {
    crit_enter();
    if (pid == current_pid) {
        //remove_process(pid);
        sched_yield();
    } else {
        //remove_process(pid);
    }

    crit_exit();
    return 1;
}

int sched_dispatcher(void) {
    printk("Dispatching process %i...\n", current_pid);

    PROCESS* this = get_process(current_pid);
    PROCESS_MAIN* entry = this->entry;

    // enter the actual program
    entry();

    printk("Process %i terminated.\n", current_pid);

    sched_kill(current_pid);
}