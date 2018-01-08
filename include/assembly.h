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
 * Deactivates all interrupts.
 */
__attribute__((always_inline)) inline void cli(void) {
    __asm__ volatile ("cli");
}

/*!
 * Reactivates all interrupts.
 */
__attribute__((always_inline)) inline void sti(void) {
    __asm__ volatile ("sti");
}

/*!
 * Pushes the contents of the EFLAGS register onto the stack.
 */
__attribute__((always_inline)) inline void pushf(void) {
    __asm__ volatile ("pushf");
}

/*!
 * Pops the contents of the EFLAGS register from the top of the stack.
 */
__attribute__((always_inline)) inline void popf(void) {
    __asm__ volatile ("popf");
}

/*!
 * Pushes the contents of the general purpose registers onto the stack.
 */
__attribute__((always_inline)) inline void pusha(void) {
    __asm__ volatile ("pusha");
}

/*!
 * Pops the contents of the general purpose registers off the top of the stack.
 */
__attribute__((always_inline)) inline void popa(void) {
    __asm__ volatile ("popa");
}

/*!
 * CPUID
 */
__attribute__((always_inline)) inline void cpuid(uint32_t eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    __asm__ volatile (  "mov %3, %%eax;"
                        "cpuid;"
                        "mov %%ebx, %0;"
                        "mov %%ecx, %1;"
                        "mov %%edx, %2"
                        : "=m" (*ebx), "=m" (*ecx), "=m" (*edx)
                        : "Nd" (eax));
}

/*!
 * Causes an interrupt
 */
#define INT(n) __asm__ volatile ("int $"  #n)

#endif