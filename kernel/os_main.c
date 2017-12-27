#include "os_text.h"
#include "os_interrupts.h"
#include "os_pic.h"

int os_init(void) {
    pic_init();
    
    interrupts_init();
    
    text_init();
}

int os_main(void) {
    const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    text_write("Hallo! :)");

    return 0;
}