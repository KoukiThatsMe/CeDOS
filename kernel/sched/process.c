#include "cedos/sched/process.h"

#define NULL ((void*)0)

PROCESS* list_head = NULL;
PROCESS* list_tail = NULL;
PROCESS_ID next_available = 0;
uint32_t process_count = 0;

/*!
 * Gets a process ID and returns the process struct in the list with the same ID.
 * \param pid Process ID to get the process struct for.
 * \return Process struct for process with ID \p pid. 
 */
PROCESS* get_process(PROCESS_ID pid) {
    if (list_tail->id == pid) { return list_tail; }

    for (PROCESS *p = list_head; p != NULL; p = p->next) {
        if (p->id == pid) { 
            return p; 
        } else if (p->id > pid) {
            return NULL;
        }
    }

    return NULL;
}

/*!
 * Adds a process struct to the list of all processes and assigns it a unique ID.
 * \param process Pointer to the process struct to add to the list.
 * \return Process ID that was assigned to the process struct.
 */
PROCESS_ID add_process(PROCESS *process, PROCESS_ID parent_pid) {
    if (list_head == NULL) {
        list_head = process;
        list_tail = process;
        process->prev = NULL;
    } else {
        list_tail->next = process;
        process->prev = list_tail;
        list_tail = process;
    }

    process->next = NULL;

    PROCESS *parent = (parent_pid == 0) ? NULL : get_process(parent_pid);

    if (parent != NULL) {
        process->next_sibling = parent->child;

        if (parent->child != NULL) {
            parent->child->prev_sibling = process;
        }

        parent->child = process;
    } else {
        process->next_sibling = NULL;
    }

    process->prev_sibling = NULL;
    process->parent = parent;

    process_count++;

    return (process->id = next_available++);
}

/*!
 * Removes the process struct with ID \p pid from the list of all processes.
 * \param pid Process ID whose process struct is to be removed from the list.
 */
void remove_process(PROCESS_ID pid) {
    PROCESS *process = get_process(pid);

    if (process == NULL) { return; }

    if (process == list_head && process == list_tail) {
        list_head = NULL;
        list_tail = NULL;
    } else if (process == list_head && process != list_tail) {
        list_head = process->next;
        list_head->prev = NULL;
    } else if (process != list_head && process == list_tail) {
        list_tail = process->prev;
        list_tail->next = NULL;
    } else {
        PROCESS *prev = process->prev, *next = process->next;
        prev->next = next;
        next->prev = prev;
    }

    PROCESS *prev_sib = process->prev_sibling, *next_sib = process->next_sibling;
    if (prev_sib != NULL) { prev_sib->next_sibling = next_sib; }
    if (next_sib != NULL) { next_sib->prev_sibling = prev_sib; }

    process_count--;
}

/*!
 * Returns the first element of the list.
 * \return First list element.
 */
PROCESS* get_first_process(void) {
    return list_head;
}

/*!
 * Returns the last element of the list.
 * \return Last list element.
 */
PROCESS* get_last_process(void) {
    return list_tail;
}

/*!
 * Returns the number of running processes.
 * \return Number of running processes.
 */
uint32_t get_process_count(void) {
    return process_count;
}