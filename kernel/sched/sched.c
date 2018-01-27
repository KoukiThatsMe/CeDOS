#include "cedos/sched/sched.h"
#include "cedos/sched/process.h"
#include "cedos/sched/sched_strats.h"

#include "cedos/mm/paging.h"

#include "cedos/drivers/console.h"
#include "cedos/drivers/speaker.h"

#include "cedos/core.h"
#include "cedos/interrupts.h"
#include "cedos/pit.h"
#include "cedos/pic.h"

#include "assembly.h"

#define KERNEL_PRIVATE_STACK (void*)(0xC0600000)
#define USER_STACK (void*)(0xC0000000)

#define PROCESS_STD_EFLAGS (0x00000286)

PROCESS* get_slot(void) {
    static PROCESS free_slots[8];
    static uint32_t index = 0;
    return &(free_slots[index++]);
}

PROCESS_ID current_pid;

PROCESS_ID get_current_process(void) {
    return current_pid;
}

int sched_dispatcher(void);

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(VIRT_ADDR code, uint32_t code_len, PROCESS_MAIN *entry, const char *name) {
    crit_enter();

    PHYS_ADDR page_dir = create_empty_page_dir();

    // copy app code
    copy_to_pdir(code, code_len, page_dir, (VIRT_ADDR)0x10000000);

    // set process context
    PROCESS *p = get_slot();
    p->name = name;
    p->page_dir = page_dir;
    p->eip = sched_dispatcher;
    p->ebp = USER_STACK;
    p->esp = USER_STACK - sizeof(SCHED_FRAME);
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
    stack_compute_checksum(&(p->checksum), &frame, &(&frame)[1]);

    PROCESS_ID pid = add_process(p, current_pid);
    //printk("Executing task %i...\n", pid);

    crit_exit();
    return pid;
}

void sched_interrupt_c(SCHED_FRAME * volatile frame, uint32_t volatile ebp) {
    //kpanic("SCHEDULER STACK INFO");
    PROCESS* current = get_process(current_pid);

    if (current_pid != 0) {
        current->esp = (uint32_t)frame;
        current->ebp = ebp;
        current->eip = frame->eip;
        current->eflags = frame->eflags;

        // save stack checksum
        stack_compute_checksum(&(current->checksum), current->esp, current->ebp);
    }

    // select next process
    current_pid = next_schedule(current_pid);

    // unblock all blocked processes
    for (PROCESS *p = get_first_process(); p != NULL; p = p->next) {
        if (p->state == PSTATE_BLOCKED) {
            p->state = PSTATE_READY;
        }
    }

    // prepare to return to process
    PROCESS* next = get_process(current_pid);
    switch_page_dir(next->page_dir);

    STACK_CHECKSUM checksum;
    stack_compute_checksum(&(checksum), next->esp, next->ebp);

    // check stack
    if (stack_compare_checksum(&(next->checksum), &(checksum))) {
        printk("STACK DAMAGED: PROCESS %i (%s), ESP %X, EBP %X\n", current_pid, get_process(current_pid)->name, next->esp, next->ebp);
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
    pit_setup_channel(PIT_CHANNEL_0, PIT_MODE_0, SCHED_INTERVAL);

    pic1_eoi();
}

void idle(void) {
    while (1) { printk("."); hlt(); }
}

extern void* sched_interrupt;

int sched_init(void) {
    // install scheduler interrupt
    install_interrupt(PIC1_IRQ(0x00), &sched_interrupt, 0x08, INT_GATE);

    current_pid = 0;

    // create idle process
    sched_exec(0, 0, idle, "idle");

    return 1;
}

void sched_yield(void) {
    crit_enter();
    PROCESS *current = get_process(current_pid);
    if (current != NULL && current->state != PSTATE_TERMINATED) {
        current->state = PSTATE_BLOCKED;
    }

    uint32_t csc = crit_stash();
    INT(0x20);
    crit_restore(csc);

    crit_exit();
}

int sched_kill(PROCESS_ID pid) {
    int success = 1;
    crit_enter();
    
    PROCESS *process = get_process(pid);
    if (process != NULL) {
        // kill all children
        for (PROCESS *child = process->child; child != NULL; child = child->next_sibling) {
            sched_kill(child->id);
        }

        remove_process(process->id);
    } else {
        success = 0;
    }

    if (get_process(current_pid) == NULL) {
        // current process has terminated
        crit_reset();
        sched_yield();
    }


    crit_exit();
    return success;
}

int sched_start(void) {
    current_pid = 0;

    // perform the first timer interrupt manually
    pic_unmask_interrupt(0);
    INT(0x20);
}

int sched_stop(void) {
    // send signals to all processes and allow them to wrap up

    // kill all processes

    // disable interrupts
    pic_mask_interrupt(0);
}

int sched_dispatcher(void) {
    //printk("Dispatching process %i...\n", current_pid);

    PROCESS* this = get_process(current_pid);
    PROCESS_MAIN* entry = this->entry;

    // enter the actual program
    entry();

    //printk("Process %i terminated.\n", current_pid);

    sched_kill(current_pid);

    // just for absolute safety
    while (1);
}