/*! \file
 * Provides the syscall interface for user processes
 */
#ifndef SYSCALL_H
#define SYSCALL_H

/*!
 * Installs the syscall interrupt to INT 0x30
 * \return 1 on success, 0 on failure
 */
int syscall_init(void);

#endif