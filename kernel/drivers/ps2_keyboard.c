#include "cedos/drivers/keyboard.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "cedos/core.h"
#include "cedos/sched/sched.h"
#include "assembly.h"

#define PS2_DATA				0x60
#define PS2_COMMAND				0x64

#define PS2_READ_CONF           0x20
#define PS2_WRTE_CONF           0x60

#define PS2_DISABLE_A           0xAD
#define PS2_DISABLE_B			0xA7

#define PS2_ENABLE_A            0xAE
#define PS2_ENABLE_B            0xA8

#define PS2_SUCCESS             0xFA
#define PS2_FAILURE             0xFC

#define BUFFER_LENGTH           (128)

/*!
 * Initializes the PS/2 keyboard.
 * \return 1 on success, 0 on fail
 */
int ps2_kb_init(void);

/*!
 * Reads a single character from the PS/2 keyboard
 * \return A single char corresponding to a key press.
 */
uint32_t ps2_kb_read(void);

//! PS/2 keyboard driver (default driver)
KB_DRIVER ps2_kb = {
    "PS/2 keyboard driver",
    ps2_kb_init,
    ps2_kb_read
};

uint8_t buffer[BUFFER_LENGTH];
uint32_t buffer_head, buffer_tail;

__attribute__((always_inline)) inline void buffer_enqueue(uint8_t value) {
    buffer[buffer_head] = value;
    buffer_head = (buffer_head + 1) % BUFFER_LENGTH;
    if (buffer_head == buffer_tail) {
        buffer_tail = (buffer_tail + 1) % BUFFER_LENGTH;
    }
}

__attribute__((always_inline)) inline uint8_t buffer_dequeue(void) {
    uint8_t res = buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_LENGTH;
    return res;
}

__attribute__((always_inline)) inline int buffer_empty(void) {
    return (buffer_head == buffer_tail);
}

__attribute__((interrupt)) void keyboard_int_handler(INTERRUPT_FRAME *frame) {
    while (inb(PS2_COMMAND) & 0x01) {
        nop(); nop(); nop(); nop();
        buffer_enqueue(inb(PS2_DATA));
        nop(); nop(); nop(); nop();
    }

    pic1_eoi();
}

int ps2_kb_init(void) {
    // clear incoming data
    inb(PS2_DATA);

    nop(); nop(); nop(); nop();

    // get keyboard configuration
    outb(PS2_READ_CONF, PS2_COMMAND);
    nop(); nop(); nop(); nop();
    uint8_t conf = inb(PS2_DATA);

    // set keyboard configuration
    conf = (conf | 0x01) & 0x75;
    outb(PS2_WRTE_CONF, PS2_COMMAND);
    nop(); nop(); nop(); nop();
    outb(conf, PS2_DATA);

    install_interrupt(PIC1_IRQ(0x01), keyboard_int_handler, 0x08, INT_GATE);

    pic_unmask_interrupt(0x01);

    return 1;
}

uint32_t ps2_kb_read(void) {
    while (buffer_empty()) {
        sched_yield();
    }

    return buffer_dequeue();
}