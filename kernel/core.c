#include "cedos/core.h"
#include "cedos/drivers/console.h"
#include "assembly.h"

CON_DRIVER *core_con;

void print_hex_char(uint8_t c) {
    static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    core_con->write_c(hex[c >> 4]);
    core_con->write_c(hex[c & 0x0F]);
}

void print_uint32(uint32_t value) {
    uint8_t* mem = (uint8_t*)(&value);
    for (int i = 0; i < 4; i++) {
        print_hex_char(mem[3-i]);
    }
}

void memdump(void* start, uint32_t size) {
    uint8_t* _start = (uint8_t*)((uint32_t)start & 0xFFFFFFF0);
    uint8_t* _end = (uint8_t*)(((uint32_t)start + size + 0xF) & 0xFFFFFFF0);

    uint32_t first_line = (uint32_t)(_start);
    uint32_t last_line = (uint32_t)(_end);

    for (uint32_t i = first_line; i < last_line; i += 0x10) {
        core_con->write_c(' ');
        print_uint32(i);
        core_con->write_c(' ');

        for (int j = 0; j < 0x10; j++) {
            uint8_t* p = (uint8_t*)(i | j);
            
            if (p >= start && p < (start + size)) {
                print_hex_char(*p);
                core_con->write_c(' ');
            } else {
                core_con->write_c(' ');
                core_con->write_c(' ');
                core_con->write_c(' ');
            }
        }
        core_con->write_c('\n');
    }
}

void stackdump(void) {
    void *esp, *ebp;
    __asm__ volatile ("mov %%esp, %0; mov %%ebp, %1" : "=m" (esp), "=m" (ebp));
    printk("STACK DUMP:\n");
    memdump(esp, (uint32_t)ebp - (uint32_t)esp);
}

void regdump(void) {
    uint32_t eax, ebx, ecx, edx,
             esi, edi, esp, ebp;
    __asm__ volatile (  "mov %%eax, %0;"
                        "mov %%ebx, %1;"
                        "mov %%ecx, %2;"
                        "mov %%edx, %3;"
                        "mov %%esi, %4;"
                        "mov %%edi, %5;"
                        "mov %%esp, %6;"
                        "mov %%ebp, %7;"
                        : "=m" (eax),
                        "=m" (ebx),
                        "=m" (ecx),
                        "=m" (edx),
                        "=m" (esi),
                        "=m" (edi),
                        "=m" (esp),
                        "=m" (ebp));

    printk(" EAX=%i EBX=%i ECX=%i EDX=%i\n", eax, ebx, ecx, edx);
    printk(" ESI=%i EDI=%i ESP=%i EBP=%i\n", esi, edi, esp, ebp);
}

void printk(const char* fmt, ...) {
    crit_enter();
    va_list args;
    va_start(args, fmt);
    uint32_t index = 0;

    enum {
        STATE_DEFAULT,
        STATE_ARGUMENT,
    } state = STATE_DEFAULT;

    while (*fmt) {
        if (state == STATE_ARGUMENT && *fmt == 'i') {
            print_uint32(va_arg(args, uint32_t));
            state = STATE_DEFAULT;
        } else if (state == STATE_ARGUMENT && *fmt == 's') {
            const char* string = va_arg(args, const char*);
            while (*string) { core_con->write_c(*string++); }
            state = STATE_DEFAULT;
        } else if (state == STATE_ARGUMENT && *fmt == '%') {
            core_con->write_c('%');
            state = STATE_DEFAULT;
        } else if (*fmt == '%') {
            state = STATE_ARGUMENT;
        } else {
            core_con->write_c(*fmt);
        }

        fmt++;
    }

    crit_exit();
}

void kpanic(const char* string) {
    printk(string);
    core_con->write_c('\n');
    // register dump / stack dump
    regdump();
    stackdump();
    while (1) {}
}

uint32_t volatile eflags = 0;
void crit_enter(void) {
    eflags = get_eflags();
    cli();
}

void crit_exit(void) {
    set_eflags(eflags);
}

int core_init(void) {
    core_con = std_con;
    return core_con->init();
}