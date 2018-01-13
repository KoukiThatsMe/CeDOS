#include "cedos/drivers/console.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "cedos/pit.h"
#include "cedos/sched.h"
#include "cedos/mm/paging.h"
#include "cedos/core.h"
#include "assert.h"

int os_init(void) {
    core_init();
    printk("Core functions initialized.\n");

    printk("Initializing PIC...");
    pic_init();
    printk("done.\n");

    printk("Initializing PIT...");
    pit_init();
    printk("done.\n");
    
    printk("Initializing interrupts...");
    interrupts_init();
    printk("done.\n");

    printk("Setting up paging...");
    paging_init();
    printk("done.\n");

    printk("Activating interrupts...");
    sti();
    printk("done.\n");

    printk("Initializing scheduler...");
    sched_init();
    printk("done.\n");

    printk("Initialization finished.\n--------------\n");

    return 1;
}

void infodump(void) {
    uint32_t res[4];
    cpuid(0, &res[0], &res[2], &res[1]);
    res[3] = 0;
    printk("CPU manufacturer: %s\n", res);
}

extern uint8_t* IDT;

void task1(void) {
    //outb(0xFE, 0x64);
    while (1) { printk("Somebody once told me\n"); hlt(); }
}

void task2(void) {
    while (1) { printk("The world is gonna roll me\n"); hlt(); }
}

void task3(void) {
    while (1) { printk("I ain't the sharpest tool in the shed.\n"); hlt(); }
}

int os_main(void) {
    //pic_unmask_interrupt(0);
    infodump();

    // create test tasks
    printk("Creating tasks.\n");
    sched_exec(create_empty_page_dir(), task1, get_eflags());
    sched_exec(create_empty_page_dir(), task2, get_eflags());
    sched_exec(create_empty_page_dir(), task3, get_eflags());

    printk("Starting scheduler.\n");
    sched_start();

    //kpanic("SIMULATED KERNEL PANIC");

    // won't be executed
    //printk("Main procedure terminating.\n");
    return 0;
}