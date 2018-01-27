#ifndef STACK_CHECK_H
#define STACK_CHECK_H

#include <stdint.h>

typedef uint32_t STACK_CHECKSUM;

void stack_compute_checksum(STACK_CHECKSUM* checksum, const void *esp, const void *ebp);
int stack_compare_checksum(STACK_CHECKSUM* a, STACK_CHECKSUM* b);

#endif