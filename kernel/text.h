/*! \file
 * Low-level driver for VGA textmode output
 */
#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

/*!
 * Prints a number of characters of a string to the display.
 * \param string String from which to print characters.
 * \param num Number of characters to print
 */
void write_n(const char *string, uint32_t num);

/*!
 * Prints a single character to the display.
 * \param c Character to print.
 */
void write_c(const char c);

/*!
 * Prints a null-terminated string to the display.
 * \param string Null-terminated string to print.
 */
void write(const char *string);

/*!
 * Clears the entire display and resets the cursor.
 */
void clear();

/*!
 * Deletes a single character of the display and moves the cursor back.
 */
void backspace(void);

#endif