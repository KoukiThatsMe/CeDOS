/*! \file
 * Defines processes, a process list and process IDs
 */
#ifndef PROCESS_H
#define PROCESS_H

#include "cedos/mm/paging.h"

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
 * Defines the prototype for process entry functions.
 */
typedef int PROCESS_MAIN(int argc, char** argv);

/*!
 * Struct that saves context information for a process.
 */
typedef struct __PROCESS {
    //! Points to the next process struct in the list.
    struct __PROCESS *next;

    //! Unique number to identify the process with.
    PROCESS_ID id;

    //! Address of the processes own page directory in physical memory.
    PHYS_ADDR page_dir;

    //! Current state of the process.
    PROCESS_STATE state;

    //! Points to the processor instruction to be executed next.
    VIRT_ADDR eip;

    //! Points to the top of the process stack.
    VIRT_ADDR esp; 

    //! Points to the base of the process stack.
    VIRT_ADDR ebp;

    //! Current value of the EFLAGS register.
    uint32_t eflags;

    //! Points to the program that is to be executed by the scheduler.
    PROCESS_MAIN *entry;
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
 * \return Process ID that was assigned to the process struct.
 */
PROCESS_ID add_process(PROCESS* process);

/*!
 * Removes the process struct with ID \p pid from the list of all processes.
 * \param pid Process ID whose process struct is to be removed from the list.
 */
void remove_process(PROCESS_ID pid);

#endif