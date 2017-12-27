/*! \file
 * Driver for VGA textmode output
 */
#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

/*!
 * Prints a number of characters of a string to the display.
 * \param string String from which to print characters.
 * \param num Number of characters to print
 */
void text_write_n(const char *string, uint32_t num);

/*!
 * Prints a single character to the display.
 * \param c Character to print.
 */
void text_write_c(const char c);

/*!
 * Prints a null-terminated string to the display.
 * \param string Null-terminated string to print.
 */
void text_write(const char *string);

/*!
 * Initializes VGA textmode
 * \return 1 on success, 0 on fail
 */
int text_init(void);

/*!
 * Clears the entire display and resets the cursor.
 */
void text_clear(void);

/*!
 * Deletes a single character of the display and moves the cursor back.
 */
void text_backspace(void);

#endif