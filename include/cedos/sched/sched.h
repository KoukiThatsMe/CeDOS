/*! \file
 * Manages the distribution of processing time among processes.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#include "cedos/sched/process.h"

#include "cedos/mm/paging.h"

#define SCHED_INTERVAL (0xFFFF)

/*!
 * Structure of the process stack when the scheduler is executed.
 */
typedef struct {
    uint32_t edi; 
    uint32_t esi; 
    uint32_t ebp; 
    uint32_t esp; 
    uint32_t edx;
    uint32_t ecx; 
    uint32_t ebx; 
    uint32_t eax; 
    uint32_t eip;
    uint32_t cs; 
    uint32_t eflags;
}__attribute__((packed)) SCHED_FRAME;

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(VIRT_ADDR code, uint32_t code_len, PROCESS_MAIN *entry, const char *name);

/*!
 * Return the ID of the current process.
 * \return ID of current process.
 */
PROCESS_ID get_current_process(void);

/*!
 * Initializes the scheduler.
 */
int sched_init(void);

/*!
 * Starts the scheduler.
 * \return 1 on success, 0 on failure.
 */
int sched_start(void);

/*!
 * Stops the scheduler.
 * \return 1 on success, 0 on failure.
 */
int sched_stop(void);

/*!
 * Returns processing time to the scheduler prematurely and blocks the 
 * process for one unit of time.
 */
void sched_yield(void);

/*!
 * Kills the process with the specified ID.
 * \param pid Process ID of the process to be killed.
 * \return 1 on success, 0 on failure.
 */
int sched_kill(PROCESS_ID pid);

/*!
 * The scheduler.
 */
void sched_interrupt_c(SCHED_FRAME * volatile frame, uint32_t volatile ebp);
#endif