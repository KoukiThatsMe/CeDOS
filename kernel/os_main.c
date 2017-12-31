#include "driv_text.h"
#include "os_interrupts.h"
#include "driv_pic.h"
#include "os_scheduler.h"

int os_init(void) {
    pic_init();
    
    interrupts_init();
    
    text_init();
}

int os_main(void) {
    const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    text_write("Hallo! :)");

    sched_exec();

    return 0;
}