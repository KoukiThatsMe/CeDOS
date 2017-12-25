#include "os_string.h"

void *memcpy (void *destination, const void *source, size_t num) {
    if (destination >= source) {
        for (uint32_t i = 0; i < num; i++) {
            ((uint8_t*)destination)[num - 1 - i] = ((uint8_t*)source)[num - 1 - i];
        }
    } else {
        for (uint32_t i = 0; i < num; i++) {
            ((uint8_t*)destination)[i] = ((uint8_t*)source)[i];
        }
    }
    return destination;
}

void *memset (void *ptr, int value, size_t num) {
    for (uint32_t i = 0; i < num; i++) {
        ((uint8_t*)ptr)[i] = (uint8_t)value;
    }
    return ptr;
}