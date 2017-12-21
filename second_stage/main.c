#include "linker.h"

void copy_kernel(void) {
    uint8_t *display = (uint8_t*)0xB8000;
    uint8_t *text = (uint8_t*)"THIS WORKS";
    
    while (*text) {
        *display++ = *text++;
        *display++ = 0x0F;
    }

    uint8_t *kernel_dest = (uint8_t*)0x00100000;
    uint8_t *kernel_src = (SS_VMA + (KERNEL_LMA - SS_LMA));
    
    for (uint32_t i = 0; i < KERNEL_SIZE; i++) {
        kernel_dest[i] = kernel_src[i];
    }
}