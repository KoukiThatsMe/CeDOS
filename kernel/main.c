#include "cedos/drivers/console.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "cedos/scheduler.h"

int os_init(void) {
    vga_con.init();
    vga_con.write_s("TTY output initialized.\n");

    vga_con.write_s("Initializing PIC...");
    pic_init();
    vga_con.write_s("done.\n");
    
    vga_con.write_s("Initializing interrupts...");
    interrupts_init();
    vga_con.write_s("done.\n");

    vga_con.write_s("Initialization finished.\n--------------\n");
}

int os_main(void) {
    //const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    vga_con.write_s("Starting scheduler.\n");
    sched_exec();

    vga_con.write_s("Main procedure terminating.\n");
    return 0;
}