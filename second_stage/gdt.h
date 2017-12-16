#ifndef GDT_H
#define GDT_H

#include "stdint.h"

typedef struct {
    uint8_t limit_0;
    uint8_t limit_8;
    uint8_t base_0;
    uint8_t base_8;
    uint8_t base_16;
    uint8_t access;
    uint8_t limit_and_flags;
    uint8_t base_24;
} __attribute__((packed)) GDT_ENTRY;

GDT_ENTRY GDT[3];

/*struct {
    uint32_t gdt_offset;
    uint16_t gdt_size;
} GDT_DESCRIPTOR = {
    (uint32_t)GDT,
    (uint16_t)(sizeof GDT)
};*/

#endif