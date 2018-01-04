/*!
 * Defines important kernel mode functions
 */
#ifndef KERNEL_H
#define KERNEL_H

#include <stdarg.h>

#include "cedos/drivers/console.h"

int core_init(void);
void printk(const char* string, ...);
void kpanic(const char* string);
void memdump(void* start, uint32_t size);

#endif