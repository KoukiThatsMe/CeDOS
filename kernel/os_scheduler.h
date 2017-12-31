/*! \file
 * Manages the distribution of processing time among processes.
 */
#ifndef OS_SCHEDULER_H
#define OS_SCHEDULER_H

#include <stdint.h>

typedef uint32_t ProcessID;

/*!
 * Executes a task.
 */
ProcessID sched_exec(void);

#endif