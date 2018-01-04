/*! \file
 * Driver for consoles.
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

/*!
 * Interface for a console device.
 */
typedef struct {
    /*!
     * Name of the keyboard.
     */
    const char* name;

    /*!
     * Initializes the console
     * \return 1 on success, 0 on fail
     */
    int (*init)(void);

    /*!
     * Prints a single character to the display.
     * \param c Character to print.
     */
    void (*write_c)(const char c);
    void (*write_n)(const char *string, uint32_t num);
    void (*write_s)(const char *string);
    
    /*!
     * Clears the entire display and resets the cursor.
     */
    void (*clear)(void);

    /*!
     * Moves the cursor to the next line.
     */
    void (*newline)(void);
    
    /*!
     * Deletes a single character of the display and moves the cursor back.
     */
    void (*backspace)(void);
} CON_DRIVER;

//! VGA console driver (default driver)
CON_DRIVER vga_con;

CON_DRIVER *std_con;

#endif