/*! \file
 * Driver for PS/2 keyboard
 */
#ifndef OS_KEYBOARD_H
#define OS_KEYBOARD_H

#include <stdint.h>

/*!
 * Initializes the keyboard.
 * \return 1 on success, 0 on fail
 */
int keyboard_init(void);

/*!
 * Reads a single character from the keyboard
 * \return A single char corresponding to a key press.
 */
uint8_t keyboard_read_c(void);

#endif