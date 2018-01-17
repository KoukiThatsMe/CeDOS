#include "cedos/drivers/console.h"

#include "cedos/sched/sched.h"
#include "cedos/sched/process.h"

#include "cedos/mm/paging.h"

#include "cedos/interrupts.h"
#include "cedos/syscall.h"
#include "cedos/pic.h"
#include "cedos/pit.h"
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

    printk("Installing syscalls...");
    syscall_init();
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

int task1(void);
int task2(void);
int task3(void);

int task1(void) {
    //outb(0xFE, 0x64);
    sched_exec(create_empty_page_dir(), task2);
    printk("  Somebody once told me\n");
    while (1) { hlt(); }
}

int task2(void) {
    //sched_yield();
    syscall(0, 0, 0, 0);
    sched_exec(create_empty_page_dir(), task3);
    printk("  The world is gonna roll me\n");
    
    while (1) { hlt(); }
}

int task3(void) {
    printk("  I ain't the sharpest tool in the shed.\n");
    while (1) { hlt(); }
}

int os_main(void) {
    //pic_unmask_interrupt(0);
    infodump();

    // create test tasks
    printk("Creating tasks.\n");
    sched_exec(create_empty_page_dir(), task1);

    printk("Starting scheduler.\n");
    sched_start();

    //kpanic("SIMULATED KERNEL PANIC");

    // won't be executed
    //printk("Main procedure terminating.\n");
    return 0;
}