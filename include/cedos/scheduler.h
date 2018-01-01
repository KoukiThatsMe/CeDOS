/*! \file
 * Manages the distribution of processing time among processes.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

typedef uint32_t ProcessID;

/*!
 * Executes a task.
 */
ProcessID sched_exec(void);

int sched_init(void);

#endif