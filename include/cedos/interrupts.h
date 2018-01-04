/*! \file
 * Sets up the interrupt descriptor table and provides functions to 
 * hook functions to interrupts.
 */
#ifndef OS_INTERRUPTS_H
#define OS_INTERRUPTS_H

#include <stdint.h>

#define INTERRUPT(name, frame) __attribute__((interrupt)) void name(INTERRUPT_FRAME *frame)
#define EXCEPTION(name, frame, err_code) __attribute__((interrupt)) void name(INTERRUPT_FRAME *frame, uword_t err_code)

/*!
 * Represents a single entry of the IDT.
 */
typedef struct {
    uint16_t offset_0;
    uint16_t selector;
    uint8_t __zero;
    uint8_t type_attr;
    uint16_t offset_16;
}__attribute__((packed)) IDT_ENTRY;

typedef struct {
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
}__attribute__((packed)) INTERRUPT_FRAME;

typedef uint32_t uword_t;

int interrupts_init(void);

#endif
