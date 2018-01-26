/*! \file
 * Wrapper for symbols defined in the linker.
 */
#ifndef LINKER_H
#define LINKER_H

#include <stdint.h>

extern uint8_t __SS_VMA;
extern uint8_t __SS_LMA;
extern uint8_t __SS_SIZE;

extern uint8_t __KERNEL_VMA;
extern uint8_t __KERNEL_LMA;
extern uint8_t __KERNEL_SIZE;

extern uint8_t __APP_VMA;
extern uint8_t __APP_LMA;
extern uint8_t __APP_SIZE;

#define SS_VMA (&__SS_VMA)
#define SS_LMA (&__SS_LMA)
#define SS_SIZE (uint32_t)(&__SS_SIZE)

#define KERNEL_VMA (&__KERNEL_VMA)
#define KERNEL_LMA (&__KERNEL_LMA)
#define KERNEL_SIZE (uint32_t)(&__KERNEL_SIZE)

#define APP_VMA (&__APP_VMA)
#define APP_LMA (&__APP_LMA)
#define APP_SIZE (uint32_t)(&__APP_SIZE)

#endif