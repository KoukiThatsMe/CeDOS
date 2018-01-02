/*!
 * Defines important kernel mode functions
 */
#ifndef KERNEL_H
#define KERNEL_H

#include "cedos/drivers/console.h"

__attribute__((always_inline)) inline void printk(const char* string) {
    vga_con.write_s(string);
}

#endif