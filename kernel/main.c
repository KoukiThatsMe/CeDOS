#include "cedos/drivers/console.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "cedos/scheduler.h"
#include "cedos/mm/paging.h"
#include "cedos/kernel.h"

int os_init(void) {
    vga_con.init();
    printk("TTY output initialized.\n");

    printk("Initializing PIC...");
    pic_init();
    printk("done.\n");
    
    printk("Initializing interrupts...");
    interrupts_init();
    printk("done.\n");

    printk("Initialization finished.\n--------------\n");

    return 1;
}

int os_main(void) {
    //const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    printk("Starting scheduler.\n");
    sched_exec(create_empty_page_dir(), (void*)0, (void*)0);

    printk("Main procedure terminating.\n");
    return 0;
}