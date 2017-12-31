/*! \file
 * Defines several functions to manipulate C strings and arrays.
 */
#ifndef STRING_H
#define STRING_H

#include <stdint.h>

/*!
 * Unsigned integral type.
 */
typedef uint32_t size_t;

/*!
 * Copies a block of memory.
 * \param destination Pointer to the destination array where the content is to be copied.
 * \param source Pointer to the source of the data to be copied.
 * \param num Number of bytes to be copied.
 * \return \p destination is returned.
 */
void *memcpy (void *destination, const void *source, size_t num);

/*!
 * Sets the \p num first bytes of the block of memory pointed to by \p ptr 
 * to the specified \p value
 * \param ptr Pointer to the block of memory to be filled.
 * \param value Value to be set (Passed as int, but converted to unsigned char)
 * \param num Number of bytes to set to \p value.
 * \return \p ptr is returned.
 */
void *memset (void *ptr, int value, size_t num);

#endif