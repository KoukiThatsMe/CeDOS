#include "driv_pic.h"
#include "assembly.h"
#include "common.h"

/*!
 * Moves irqs to appropriate addresses and enables all PIC interrupts
 * \return 1 on success, 0 on fail
 */
volatile int pic_init(void) {
    // start initialization sequence
    outb(0x11, PIC1_COMMAND);
    outb(0x11, PIC2_COMMAND);
    nop(); nop(); nop(); nop();

    // set offsets
    outb(PIC1_OFFSET, PIC1_DATA);
    outb(PIC2_OFFSET, PIC2_DATA);
    nop(); nop(); nop(); nop();

    // set wiring info
    outb(0b00000100, PIC1_DATA);
    outb(0b00000010, PIC2_DATA);
    nop(); nop(); nop(); nop();

    // set operating mode (8086)
    outb(0x01, PIC1_DATA);
    outb(0x01, PIC2_DATA);
    nop(); nop(); nop(); nop();

    // clear interrupt masks
    outb(0xff, PIC1_DATA);
    outb(0xff, PIC2_DATA);
}

/*!
 * Unmasks interrupt number \p irq.
 * \param irq Number of the interrupt to unmask.
 * \return 1 on success, 0 on fail
 */
int pic_unmask_interrupt(int irq) {
    uint16_t port;

    if (irq >= 8) {
        port = PIC1_DATA;    
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    uint8_t mask = inb(port);
    mask &= ~(1 << irq);
    outb(mask, port);
}

/*!
 * Masks interrupt number \p irq.
 * \param irq Number of the interrupt to mask.
 * \return 1 on success, 0 on fail
 */
int pic_mask_interrupt(int irq) {
    uint16_t port;
    
    if (irq >= 8) {
        port = PIC1_DATA;    
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    uint8_t mask = inb(port);
    mask |= 1 << irq;
    outb(mask, port);
}
