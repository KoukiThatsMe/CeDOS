#include <stdint.h>

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

int start(void) {
    uint32_t eip, esp, ebp;
    __asm__ volatile ("call jump; jump: pop %0; mov %%esp, %1; mov %%ebp, %2" : "=m" (eip), "=m" (esp), "=m" (ebp));

    syscall(0, eip, esp, ebp);

    while(1);
}