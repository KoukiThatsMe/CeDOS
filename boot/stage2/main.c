#include "linker.h"
#include "paging.h"

/*
#define VGA_TEXTMODE_COLUMNS 80
#define VGA_TEXTMODE_LINES 25
#define VGA_TEXTMODE_CELLS (VGA_TEXTMODE_COLUMNS * VGA_TEXTMODE_LINES)

#define VGA_TEXT_COLOR 0x02

uint8_t const *display_start = (uint8_t*)0xB8000;
uint8_t *display = (uint8_t*)0xB8000;

void simple_clear(void) {
    display = (uint8_t*)display_start;

    for (int i = 0; i < VGA_TEXTMODE_CELLS; i++) {
        display[2 * i] = 0x00;
        display[2 * i + 1] = 0x00;
    }
}

void simple_print(const char *src) {
    while (*src) {
        *display++ = *src++;
        *display++ = VGA_TEXT_COLOR;
    }
}

void simple_println(const char *src) {
    simple_print(src);

    while ((display - display_start) % (2 * VGA_TEXTMODE_COLUMNS)) {
        display++;
        display++;
    }
}
*/

void copy_kernel(void) {
    uint8_t *kernel_dest = (uint8_t*)0x00100000;
    uint8_t *kernel_src = (SS_VMA + (KERNEL_LMA - SS_LMA));
    
    for (uint32_t i = 0; i < KERNEL_SIZE + APP_SIZE; i++) {
        kernel_dest[i] = kernel_src[i];
    }
}