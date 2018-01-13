#include "cedos/interrupts.h"
#include "cedos/drivers/console.h"
#include "cedos/pic.h"
#include "cedos/core.h"
#include "cedos/pit.h"
#include "cedos/sched.h"

#define array_sizeof(array) (sizeof(array)/sizeof(array[0]))
#define NULL ((void*)0)

#define MAKE_IDT_ENTRY(func, selector, type) { \
        (uint16_t)(func), \
        (uint16_t)(selector), \
        0, \
        (uint8_t)(type), \
        (uint16_t)(0xC000) \
    }

INTERRUPT(default_isr, frame) {
    printk("interrupt was issued\n");
}

INTERRUPT(breakpoint_isr, frame) {
    printk("BREAKPOINT WAS HIT\n");
}

EXCEPTION(double_fault_isr, frame, error_code) {
    kpanic("CRITICAL: DOUBLE FAULT");
}

EXCEPTION(gpf_isr, frame, error_code) {
    kpanic("CRITICAL: GENERAL PROTECTION FAULT");
}

INTERRUPT(pic1_handler, frame) {
    printk("PIC1 INTERRUPT\n");
    pic1_eoi();
}

INTERRUPT(pic2_handler, frame) {
    printk("PIC2 INTERRUPT\n");
    pic2_eoi();
}

IDT_ENTRY IDT[INTERRUPT_COUNT];

void install_interrupt(int num, void* func, uint16_t selector, uint8_t type) {
    IDT[num].offset_0 = (uint16_t)((uint32_t)func);
    IDT[num].selector = selector;
    IDT[num].__zero = 0;
    IDT[num].type_attr = type;
    IDT[num].offset_16 = (uint16_t)((uint32_t)(func) >> 16);
}

struct {
    uint16_t limit;
    void* offset;
}__attribute__((packed)) IDT_DESC = {
    sizeof(IDT),
    IDT
};

extern void* sched_interrupt;

int interrupts_init(void) {
    for (uint32_t i = 0; i < INTERRUPT_COUNT; i++) {
        if (i == 0x03) {
            install_interrupt(i, breakpoint_isr, 0x08, INT_GATE);
        } else if (i == 0x08) {
            install_interrupt(i, double_fault_isr, 0x08, INT_GATE);
        } else if (i == 0x0d) {
            install_interrupt(i, gpf_isr, 0x08, INT_GATE);
        } else if (i == 0x20) {
            install_interrupt(i, &sched_interrupt, 0x08, INT_GATE);
        } else if (i >= 0x21 || i < 0x28) {
            install_interrupt(i, pic1_handler, 0x08, INT_GATE);
        } else if (i >= 0x28 || i < 0x30) {
            install_interrupt(i, pic2_handler, 0x08, INT_GATE);
        } else {
            install_interrupt(i, default_isr, 0x08, INT_GATE);
        }
    }
    
    __asm__ volatile (
            "lidt (%0)\n" : : 
            "m" (IDT_DESC)
        );

    return 1;
}