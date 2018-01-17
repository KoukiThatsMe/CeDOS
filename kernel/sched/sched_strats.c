#include "cedos/sched/process.h"

#define NULL ((void*)0)

PROCESS_ID next_schedule(PROCESS_ID current) {
    PROCESS* process = get_process(current);

    if (process != NULL && process->next != NULL) {
        return process->next->id;
    } else {
        PROCESS *first = get_first_process();
        if (first != NULL && first->id != 0) {
            return first->id;
        } else if (first != NULL && first->next != NULL) {
            return first->next->id;
        } else {
            return 0;
        }
    }
}