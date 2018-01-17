#include "cedos/interrupts.h"

INTERRUPT(syscall_interrupt, frame) {
    printk("SYSCALL\n");
}

/*!
 * Installs the syscall interrupt to INT 0x30
 */
int syscall_init(void) {
    install_interrupt(0x30, syscall_interrupt, 0x08, INT_GATE);
    return 1;
}