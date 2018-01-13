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
    __asm__ volatile ("pusha" : : : "eax", "ebx", "ecx", "edx", "esi", "edi");
}

/*!
 * Pops the contents of the general purpose registers off the top of the stack.
 */
__attribute__((always_inline)) inline void popa(void) {
    __asm__ volatile ("popa" : : : "eax", "ebx", "ecx", "edx", "esi", "edi");
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
 * Used to find out the current value of the flags register.
 * \return Value of the EFLAGS-register.
 */
__attribute((always_inline)) inline uint32_t get_eflags() {
    uint32_t eflags;
    __asm__ volatile ("pushf; pop %%eax; mov %%eax, %0" : "=m" (eflags) : : "eax");
    return eflags;
}

/*!
 * Sets the value of the EFLAGS-register.
 * \param eflags New value of the EFLAGS-register.
 */
__attribute__((always_inline)) inline void set_eflags(uint32_t eflags) {
    __asm__ volatile ("mov %0, %%eax; push %%eax; popf" : : "m" (eflags) : "eax");
}

/*!
 * Waits for the next interrupt.
 */
__attribute__((always_inline)) inline void hlt(void) {
    __asm__ volatile ("hlt");
}

/*!
 * Causes an interrupt
 */
#define INT(n) __asm__ volatile ("int $"  #n)

#endif