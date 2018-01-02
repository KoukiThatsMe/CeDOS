#include "cedos/process.h"

#define NULL ((void*)0)

PROCESS* list_head;
PROCESS_ID current_process = 0;
PROCESS_ID next_available = 0;

PROCESS* get_process(PROCESS_ID pid) {
    for (PROCESS *p = list_head; p != NULL; p = p->next) {
        if (p->id == pid) { 
            return p; 
        } else if (p->id > pid) {
            return NULL;
        }
    }

    return NULL;
}

PROCESS_STATE get_process_state(PROCESS_ID pid) {
    PROCESS* p = get_process(pid);
    if (p == NULL) {
        return PSTATE_TERMINATED;
    } else {
        return p->state;
    }
}

PROCESS_ID get_current_process(void) {
    return current_process;
}

PROCESS_ID add_process(PROCESS* process) {
    for (PROCESS **p = &list_head; *p != NULL; p = &((*p)->next)) {
        if ((*p)->next == NULL) {
            (*p)->next = process;
            process->id = (*p)->id + 1;
            process->next = NULL;
            return process->id;
        } else if ((*p)->next->id > (*p)->id + 1) {
            (*p)->next = process;
            process->id = (*p)->id + 1;
            process->next = (*p);
            return process->id;
        }
    }

    list_head = process;
    process->next = NULL;
    process->id = 0;
    return 0;
}

void remove_process(PROCESS_ID pid) {
    for (PROCESS **p = &list_head; (*p) != NULL; p = &((*p)->next)) {
        if ((*p)->next != NULL && (*p)->next->id == pid) {
            (*p)->next = (*p)->next->next;
            return;
        }
    }
}