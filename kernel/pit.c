#include "cedos/pit.h"
#include "assembly.h"
#include "cedos/pic.h"

#define PIT_ACCESS_MODE_LO_HI (0b11 << 4)
#define PIT_COMMAND_PORT ((uint16_t)0x43)

void pit_setup_channel(PIT_CHANNEL channel, PIT_MODE mode, uint16_t freq_div) {
    pushf();
    cli();
    outb(((uint8_t)channel << 6) | PIT_ACCESS_MODE_LO_HI | (uint8_t)mode, PIT_COMMAND_PORT);
    nop(); nop(); nop(); nop();

    outb((uint8_t)(freq_div & 0xFF), 0x40 + (uint16_t)channel);
    nop(); nop(); nop(); nop();
    
    outb((uint8_t)(freq_div >> 8), 0x40 + (uint16_t)channel);
    popf();
}

void pit_init(void) {
    return 1;
}