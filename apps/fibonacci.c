#include "assembly.h"

#include <stdint.h>

void fib(void) {
    uint32_t a = 0, b = 1;
    while (1) {
        uint32_t tmp = a + b;
        a = b;
        b = tmp;
        syscall(0, a, 0, 0);
        hlt();
    }
}