#include "cedos/interrupts.h"
#include "cedos/core.h"

void test(uint32_t ebx, uint32_t ecx, uint32_t edx) {
    printk("SYSCALL 0x01: EBX=%X ECX=%X EDX=%X\n", ebx, ecx, edx);
}

void* SYSCALL_TABLE[] = { hard_reset, test };

extern void syscall_interrupt(void);

/*!
 * Installs the syscall interrupt to INT 0x30
 */
int syscall_init(void) {
    install_interrupt(0x30, &syscall_interrupt, 0x08, INT_GATE);
    return 1;
}