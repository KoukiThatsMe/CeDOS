#include "cedos/interrupts.h"
#include "cedos/drivers/console.h"
#include "cedos/pic.h"

#define INT_GATE (0b10001110)
#define TRAP_GATE (0b10001111)
#define SYSCALL_INTERRUPT (0b11101110)

#define array_sizeof(array) (sizeof(array)/sizeof(array[0]))

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

EXCEPTION(page_fault_isr, frame, error_code) {
    volatile uint32_t faulty_addr;
    __asm__ volatile ("mov %%cr2, %0" : "=a" (faulty_addr));
    printk("PAGE FAULT: %i", faulty_addr);
    while (1) {}
    // dump registers to stdout
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

IDT_ENTRY IDT[31];

void install_interrupt(int index, void* func, uint16_t selector, uint8_t type) {
    IDT[index].offset_0 = (uint16_t)((uint32_t)func);
    IDT[index].selector = selector;
    IDT[index].__zero = 0;
    IDT[index].type_attr = type;
    IDT[index].offset_16 = (uint16_t)((uint32_t)(func) >> 16);
}

struct {
    uint16_t limit;
    void* offset;
}__attribute__((packed)) IDT_DESC = {
    sizeof(IDT),
    IDT
};

int interrupts_init(void) {
    for (uint32_t i = 0; i < array_sizeof(IDT); i++) {
        if (i == 0x03) {
            install_interrupt(i, breakpoint_isr, 0x08, INT_GATE);
        } else if (i == 0x08) {
            install_interrupt(i, double_fault_isr, 0x08, INT_GATE);
        } else if (i == 0x0e) {
            install_interrupt(i, page_fault_isr, 0x08, INT_GATE);
        } else if (i == 0x0d) {
            install_interrupt(i, gpf_isr, 0x08, INT_GATE);
        } else if (i >= 0x20 || i < 0x28) {
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