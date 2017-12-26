#include "os_interrupts.h"
#include "os_text.h"

#define HARDWARE_INTERRUPT (0b10001110)
#define SYSCALL_INTERRUPT (0b11101110)

#define array_sizeof(array) (sizeof(array)/sizeof(array[0]))

#define MAKE_IDT_ENTRY(func, selector, type) { \
        (uint16_t)(func), \
        (uint16_t)(selector), \
        0, \
        (uint8_t)(type), \
        (uint16_t)(0xC000) \
    }
    
struct interrupt_frame {

};

__attribute__((interrupt)) void default_isr(struct interrupt_frame *frame) {
    write("interrupt was issued\n");
}

__attribute__((interrupt)) void breakpoint_isr(struct interrupt_frame *frame) {
    write("BREAKPOINT WAS HIT\n");
    // dump registers to stdout
}

__attribute__((interrupt)) void double_fault_isr(struct interrupt_frame *frame) {
    write("CRITICAL: DOUBLE FAULT\n");
    while (1) {}
}

IDT_ENTRY IDT[31];

void install_interrupt(int index, void* func, uint16_t selector, uint8_t type) {
    IDT[index].offset_0 = (uint16_t)((uint32_t)func);
    IDT[index].selector = selector;
    IDT[index].__zero = 0;
    IDT[index].type_attr = type;
    IDT[index].offset_16 = (uint16_t)((uint32_t)(func) >> 16);
    //if (index == 7) { while(1) {} }
}

struct {
    uint16_t limit;
    void* offset;
}__attribute__((packed)) IDT_DESC = {
    sizeof(IDT),
    IDT
};

void init_interrupts(void) {
    for (uint32_t i = 0; i < array_sizeof(IDT); i++) {
        switch (i) {
            case 0x03:
            install_interrupt(i, breakpoint_isr, 0x08, HARDWARE_INTERRUPT);
            case 0x08:
            install_interrupt(i, double_fault_isr, 0x08, HARDWARE_INTERRUPT);
            break;
            default:
            install_interrupt(i, default_isr, 0x08, HARDWARE_INTERRUPT);
        }
    }

    __asm__ volatile (
            "lidt (%0)\n"
            "sti" : : 
            "m" (IDT_DESC)
        );
}