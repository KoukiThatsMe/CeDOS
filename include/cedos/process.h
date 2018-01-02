/*! \file
 * Defines processes, a process list and process IDs
 */
#ifndef PROCESS_H
#define PROCESS_H

#include "cedos/mm/paging.h"

typedef enum {
    PSTATE_READY,
    PSTATE_RUNNING,
    PSTATE_BLOCKED,
    PSTATE_TERMINATED
} PROCESS_STATE;

typedef uint32_t PROCESS_ID;

struct __PROCESS {
    struct __PROCESS *next;
    PROCESS_ID id;

    PHYS_ADDR page_dir;

    PROCESS_STATE state;
    VIRT_ADDR eip;
    VIRT_ADDR esp;

    uint32_t eax, ebx, ecx, edx;
};

typedef struct __PROCESS PROCESS;

PROCESS* get_process(PROCESS_ID pid);
PROCESS_STATE get_process_state(PROCESS_ID pid);
PROCESS_ID get_current_process(void);

PROCESS_ID add_process(PROCESS* process);
void remove_process(PROCESS_ID pid);

#endif