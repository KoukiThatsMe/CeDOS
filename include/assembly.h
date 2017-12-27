/*! \file
 * Wrapper for low-level assembler instructions.
 */
#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdint.h>

/*!
 * Sends a single byte to the specified port.
 * \param msg The byte to be sent.
 * \param port The destination port.
 */
__attribute((always_inline)) inline void outb(uint8_t msg, uint16_t port) {
    __asm__ volatile ("outb %0, %1" : : "a" (msg), "Nd" (port));
}

/*!
 * Retrieves a single byte from the specified port.
 * \param port The source port.
 * \return The recieved byte.
 */
__attribute((always_inline)) inline uint8_t inb(uint16_t port) {
    uint8_t msg;
    __asm__ volatile ("inb %1, %0" : "=a" (msg) : "Nd" (port));
    return msg;
}

/*!
 * nop
 */
__attribute__((always_inline)) inline void nop(void) {
    __asm__ volatile ("nop");
}

/*!
 * Causes an interrupt
 */
#define int(n) __asm__ volatile ("int $"  #n)

#endif