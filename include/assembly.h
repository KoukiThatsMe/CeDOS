#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdint.h>

__attribute((always_inline)) inline void outb(uint8_t msg, uint16_t port) {
    __asm__ volatile ("outb %0, %1" : "=a" (msg), "=Nd" (port));
}

__attribute((always_inline)) inline uint8_t inb(uint16_t port) {
    uint8_t msg;
    __asm__ volatile ("inb %1, %0" : "=a" (msg) : "Nd" (port));
    return msg;
}

#endif