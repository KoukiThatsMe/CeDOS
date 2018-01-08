#include "cedos/drivers/console.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "cedos/pit.h"
#include "cedos/scheduler.h"
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

    printk("Setting up timer interrupts...");
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

int os_main(void) {
    //pic_unmask_interrupt(0);
    infodump();

    printk("Starting scheduler.");
    sched_start();

    //kpanic("SIMULATED KERNEL PANIC");

    // won't be executed
    //printk("Main procedure terminating.\n");
    return 0;
}