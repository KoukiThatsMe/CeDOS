#ifndef ASSEMBLY_H
#define ASSEMBLY_H

/*!
 * Performs a syscall.
 */
__attribute__((always_inline)) inline int syscall(int eax, int ebx, int ecx, int edx) {
    int res;
    __asm__ volatile (  "mov %1, %%eax;"
                        "mov %2, %%ebx;"
                        "mov %3, %%ecx;"
                        "mov %4, %%edx;"
                        "int $0x30;"
                        "mov %%eax, %0;" : "=m" (res) : "" (eax), "" (ebx), "" (ecx), "" (edx) : "eax", "ebx", "ecx", "edx");
    return res;
}

/*!
 * Waits for the next interrupt.
 */
__attribute__((always_inline)) inline void hlt(void) {
    __asm__ volatile ("hlt");
}

#endif