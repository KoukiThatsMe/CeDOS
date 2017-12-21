#include "linker.h"

uint8_t *display = (uint8_t*)0xB8000;

void simple_clear(void) {
    display = (uint8_t*)0xB8000;

    for (int i = 0; i < 80 * 25; i++) {
        display[2 * i] = 0x00;
        display[2 * i + 1] = 0x00;
    }
}

void simple_print(const char* src) {
    while (*src) {
        *display++ = *src++;
        *display++ = 0x02;
    }
}

void copy_kernel(void) {
    simple_clear();
    simple_print("COPYING_KERNEL...");

    uint8_t *kernel_dest = (uint8_t*)0x00100000;
    uint8_t *kernel_src = (SS_VMA + (KERNEL_LMA - SS_LMA));
    
    for (uint32_t i = 0; i < KERNEL_SIZE; i++) {
        kernel_dest[i] = kernel_src[i];
    }

    simple_print("DONE.");
}