#include "cedos/sched/stack_check.h"

STACK_CHECKSUM stack_check(const void *esp, const void *ebp) {
    STACK_CHECKSUM sum = 0;
    
    for (uint32_t *p = esp; p < ebp; p = &p[1]) {
        sum ^= *p;
    }

    return sum;
}