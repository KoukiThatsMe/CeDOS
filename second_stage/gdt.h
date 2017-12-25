/*! \file
 * Sets up the global descriptor table (GDT).
 */
#ifndef GDT_H
#define GDT_H

#include "stdint.h"

/*!
 * Represents a single segment descriptor within the GDT.
 */
typedef struct {
    //! Bytes 0-7 of the segment limit
    uint8_t limit_0;

    //! Bytes 8-15 of the segment limit
    uint8_t limit_8;

    //! Bytes 0-7 of the segment base
    uint8_t base_0;

    //! Bytes 8-15 of the segment base
    uint8_t base_8;

    //! Bytes 16-23 of the segment base
    uint8_t base_16;

    //! Access bytes: Contains information regulating access to the segment
    uint8_t access;

    //! Bytes 16-19 of the segment limit and flags
    uint8_t limit_and_flags;

    //! Bytes 24-31 of the segment base
    uint8_t base_24;
} __attribute__((packed)) GDT_ENTRY;

/*!
 * The GDT.
 */
GDT_ENTRY GDT[5];

#endif