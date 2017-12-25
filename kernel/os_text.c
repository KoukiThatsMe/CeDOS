#include "linker.h"
#include "assembly.h"

#define VGA_TEXTMODE_COLUMNS 80
#define VGA_TEXTMODE_LINES 25
#define VGA_TEXTMODE_CELLS (VGA_TEXTMODE_COLUMNS * VGA_TEXTMODE_LINES)
#define VGA_TEXTMODE_BPC 2
#define VGA_MEM_POS(line, column) ((line) * VGA_TEXTMODE_COLUMNS * VGA_TEXTMODE_BPC + (column) * VGA_TEXTMODE_BPC)

#define VGA_TEXTMODE_MEM ((uint8_t*)0xB8000)

#define VGA_INDEX_REG ((uint16_t)0x3D4)
#define VGA_DATA_REG ((uint16_t)0x3D5)

uint32_t line = 0;
uint32_t column = 0;
uint8_t color = 0x0F;

__attribute((always_inline)) inline void set_char(char value) {
    VGA_TEXTMODE_MEM[VGA_MEM_POS(line, column)] = value;
    VGA_TEXTMODE_MEM[VGA_MEM_POS(line, column) + 1] = color;
}

__attribute((always_inline)) inline void lfcr() {
    if (line < VGA_TEXTMODE_LINES) {
        line++;
    } else {
        line = VGA_TEXTMODE_LINES - 1;
    }
    column = 0;
}

__attribute((always_inline)) inline void write_char(char value) {
    switch (value) {
        case '\n':
            lfcr();
            break;
        case '\0':
            break;
        default:
            set_char(value);
            column++;
            break;
    }

    if (column >= VGA_TEXTMODE_COLUMNS) {
        lfcr();
    }
}

__attribute((always_inline)) inline void set_cursor(uint16_t line, uint16_t column) {
    uint16_t pos = line * VGA_TEXTMODE_COLUMNS + column;
    outb(0x0F, VGA_INDEX_REG);
    outb((uint8_t)(pos & 0xFF), VGA_DATA_REG);
    outb(0x0E, VGA_INDEX_REG);
    outb((uint8_t)(pos >> 8), VGA_DATA_REG);
}

void backspace(void) {
    if (column == 0 && line > 0) {
        line--;
        column = VGA_TEXTMODE_COLUMNS - 1;
    } else if (column > 0) {
        column--;
    }

    set_char(0);
    set_cursor(line, column);
}

void write_n(const char *string, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        write_char(string[0]);
    }
    set_cursor(line, column);
}

void write(const char *string) {
    while (*string) {
        write_char(*(string++));
    }
    set_cursor(line, column);
}

void write_c(const char c) {
    write_char(c);
    set_cursor(line, column);
}

void enable_cursor(void) {
    outb(0x0A, VGA_INDEX_REG);
    outb((inb(VGA_DATA_REG) & 0x0C) | 0x00, VGA_DATA_REG);
    outb(0x0B, VGA_INDEX_REG);
    outb((inb(VGA_DATA_REG) & 0xE0) | 0x0F, VGA_DATA_REG);
}

void clear(void) {
    for (int i = 0; i < VGA_TEXTMODE_CELLS; i++) {
        VGA_TEXTMODE_MEM[2 * i] = 0;
        VGA_TEXTMODE_MEM[2 * i + 1] = 0;
    }

    line = 0;
    column = 0;
    enable_cursor();
    set_cursor(line, column);
}
