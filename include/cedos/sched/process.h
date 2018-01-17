/*! \file
 * Defines processes, a process list and process IDs
 */
#ifndef PROCESS_H
#define PROCESS_H

#include "cedos/mm/paging.h"
#include "cedos/sched/stack_check.h"

/*!
 * Defines all possible states for processes.
 */
typedef enum {
    PSTATE_TERMINATED,
    PSTATE_READY,
    PSTATE_RUNNING,
    PSTATE_BLOCKED
} PROCESS_STATE;

/*!
 * Type for Process IDs.
 */
typedef uint32_t PROCESS_ID;

/*!
 * Type for Process IDs.
 */
typedef uint32_t PROCESS_PRIO;

/*!
 * Defines the prototype for process entry functions.
 */
typedef int PROCESS_MAIN(void);

/*!
 * Struct that saves context information for a process.
 */
typedef struct __PROCESS {
    //! Points to the next process struct in the process list.
    struct __PROCESS *next;

    //! Points to the previous process struct in the process list.
    struct __PROCESS *prev;

    //! Points to the processes first child process.
    struct __PROCESS *child;

    //! Points to the processes parent process.
    struct __PROCESS *parent;

    //! Points to the processes next sibling process.
    struct __PROCESS *next_sibling;

    //! Points to the processes previous sibling process.
    struct __PROCESS *prev_sibling;

    //! Unique number to identify the process with.
    PROCESS_ID id;

    //! Name of the process.
    const char *name;

    //! Current state of the process.
    PROCESS_STATE state;

    //! Process priority.
    PROCESS_PRIO priority;

    //! Stack checksum
    STACK_CHECKSUM checksum;

    //! Points to the program that is to be executed by the scheduler.
    PROCESS_MAIN *entry;

    //! Address of the processes own page directory in physical memory.
    PHYS_ADDR page_dir;

    //! Points to the processor instruction to be executed next.
    VIRT_ADDR eip;

    //! Points to the top of the process stack.
    VIRT_ADDR esp; 

    //! Points to the base of the process stack.
    VIRT_ADDR ebp;

    //! Current value of the EFLAGS register.
    uint32_t eflags;
} PROCESS;

/*!
 * Gets a process ID and returns the process struct in the list with the same ID.
 * \param pid Process ID to get the process struct for.
 * \return Process struct for process with ID \p pid. 
 */
PROCESS* get_process(PROCESS_ID pid);

/*!
 * Adds a process struct to the list of all processes and assigns it a unique ID.
 * \param process Pointer to the process struct to add to the list.
 * \param parent_pid PID of the parent process.
 * \return Process ID that was assigned to the process struct.
 */
PROCESS_ID add_process(PROCESS *process, PROCESS_ID parent_pid);

/*!
 * Removes the process struct with ID \p pid from the list of all processes.
 * \param pid Process ID whose process struct is to be removed from the list.
 */
void remove_process(PROCESS_ID pid);

/*!
 * Returns the first element of the list.
 * \return First list element.
 */
PROCESS* get_first_process(void);

/*!
 * Returns the last element of the list.
 * \return Last list element.
 */
PROCESS* get_last_process(void);

/*!
 * Returns the number of running processes.
 * \return Number of running processes.
 */
uint32_t get_process_count(void);

#endif