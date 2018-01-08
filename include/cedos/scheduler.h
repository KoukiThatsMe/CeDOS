/*! \file
 * Manages the distribution of processing time among processes.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#include "cedos/process.h"
#include "cedos/mm/paging.h"

/*!
 * Executes a task.
 */
PROCESS_ID sched_exec(PHYS_ADDR page_dir, VIRT_ADDR eip, VIRT_ADDR esp);

/*!
 * Initializes the scheduler.
 */
int sched_init(void);

/*!
 * Starts the scheduler.
 */
int sched_start(void);

#endif