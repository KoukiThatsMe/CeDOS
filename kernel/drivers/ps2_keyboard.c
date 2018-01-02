#include "cedos/drivers/keyboard.h"
#include "cedos/interrupts.h"
#include "cedos/pic.h"
#include "assembly.h"

/*!
 * Initializes the PS/2 keyboard.
 * \return 1 on success, 0 on fail
 */
int ps2_kb_init(void);

/*!
 * Reads a single character from the PS/2 keyboard
 * \return A single char corresponding to a key press.
 */
uint8_t ps2_kb_read(void);

//! PS/2 keyboard driver (default driver)
KB_DRIVER ps2_kb = {
    "PS/2 keyboard driver",
    ps2_kb_init,
    ps2_kb_read
};

__attribute__((interrupt)) void keyboard_int_handler(INTERRUPT_FRAME *frame) {

}

int ps2_kb_init(void) {
    return 1;
}

uint8_t ps2_kb_read(void) {
    return 0;
}