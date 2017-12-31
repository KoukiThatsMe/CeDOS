#include "drivers/text.h"
#include "os_interrupts.h"
#include "drivers/pic.h"
#include "os_scheduler.h"

int os_init(void) {
    text_init();
    text_write("TTY output initialized.\n");

    text_write("Initializing PIC...");
    pic_init();
    text_write("done.\n");
    
    text_write("Initializing interrupts...");
    interrupts_init();
    text_write("done.\n");

    text_write("Initialization finished.\n--------------\n");
}

int os_main(void) {
    //const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    text_write("Starting scheduler.\n");
    sched_exec();

    text_write("Main procedure terminating.\n");
    return 0;
}