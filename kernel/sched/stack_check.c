#include "cedos/sched/stack_check.h"

void stack_compute_checksum(STACK_CHECKSUM* checksum, const void *esp, const void *ebp) {
    *checksum = 0;
    
    for (uint32_t *p = esp; p < ebp; p = &p[1]) {
        *checksum ^= *p;
    }
}

int stack_compare_checksum(STACK_CHECKSUM* a, STACK_CHECKSUM* b) {
    return (a == b);
}