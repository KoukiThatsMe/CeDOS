#include "assembly.h"

#include <stdint.h>

extern void fib(void);

int start(void) {
    uint32_t eip, esp, ebp;
    __asm__ volatile ("call jump; jump: pop %0; mov %%esp, %1; mov %%ebp, %2" : "=m" (eip), "=m" (esp), "=m" (ebp));

    syscall(0, eip, esp, ebp);

    fib();

    while(1);
}