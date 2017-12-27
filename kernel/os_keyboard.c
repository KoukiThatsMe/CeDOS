#include "os_keyboard.h"
#include "os_interrupts.h"
#include "os_pic.h"
#include "assembly.h"



/*!
 * Initializes the keyboard.
 * \return 1 on success, 0 on fail
 */
int keyboard_init(void) {

}

/*!
 * Reads a single character from the keyboard
 * \return A single char corresponding to a key press.
 */
uint8_t keyboard_read_c(void) {
    static uint32_t keycode = 0;

    keycode <<= 8;
}

__attribute__((interrupt)) volatile void keyboard_int_handler(INTERRUPT_FRAME *frame) {

}