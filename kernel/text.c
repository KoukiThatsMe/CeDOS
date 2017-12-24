#include "linker.h"

#define VGA_TEXTMODE_COLUMNS 80
#define VGA_TEXTMODE_LINES 25
#define VGA_TEXTMODE_CELLS (VGA_TEXTMODE_COLUMNS * VGA_TEXTMODE_LINES)
#define VGA_TEXTMODE_BPC 2
#define VGA_MEM_POS(line, column) ((line) * VGA_TEXTMODE_COLUMNS * VGA_TEXTMODE_BPC + (column) * VGA_TEXTMODE_BPC)

#define VGA_TEXTMODE_MEM ((uint8_t*)0xB8000)

uint32_t line = 0;
uint32_t column = 0;
uint8_t color = 0x0F;

void lfcr() {
    line++;
    column = 0;
}

void write_char(char value) {
    switch (value) {
        case '\n':
            lfcr();
            break;
        case '\0':
            break;
        default:
            VGA_TEXTMODE_MEM[VGA_MEM_POS(line, column)] = value;
            VGA_TEXTMODE_MEM[VGA_MEM_POS(line, column) + 1] = color;
            column++;
            break;
    }

    if (column >= VGA_TEXTMODE_COLUMNS) {
        lfcr();
    }
}

void write_n(const char *string, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        write_char(string[0]);
    }
}

void write(const char *string) {
    while (*string) {
        write_char(*(string++));
    }
}

void clear() {
    for (int i = 0; i < VGA_TEXTMODE_CELLS; i++) {
        VGA_TEXTMODE_MEM[2 * i] = 0;
        VGA_TEXTMODE_MEM[2 * i + 1] = 0;
    }

    line = 0;
    column = 0;
}