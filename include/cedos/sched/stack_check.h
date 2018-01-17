#ifndef STACK_CHECK_H
#define STACK_CHECK_H

#include <stdint.h>

typedef uint32_t STACK_CHECKSUM;

STACK_CHECKSUM stack_check(const void *esp, const void *ebp);

#endif