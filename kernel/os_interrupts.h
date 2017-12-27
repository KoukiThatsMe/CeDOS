/*! \file
 * Sets up the interrupt descriptor table and provides functions to 
 * hook functions to interrupts.
 */
#ifndef OS_INTERRUPTS_H
#define OS_INTERRUPTS_H

 #include <stdint.h>

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
    
} INTERRUPT_FRAME;

void interrupts_init(void);

#endif
