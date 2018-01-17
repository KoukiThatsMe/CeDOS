/*! \file
 * Provides the used scheduling strategy.
 */
#ifndef SCHED_STRATS_H
#define SCHED_STRATS_H

/*!
 * Returns the process ID that is to be scheduled next.
 * \param current Currently running process.
 * \return Next scheduled process.
 */
PROCESS_ID next_schedule(PROCESS_ID current);

#endif