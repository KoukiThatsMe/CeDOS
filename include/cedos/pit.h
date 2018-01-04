/*! \file
 * Provides an interface for the programmable interval timer (PIT).
 */
#ifndef PIT_H
#define PIT_H

#include <stdint.h>

/*!
 * Specifies the accessed PIT channel.
 */
typedef enum {
    PIT_CHANNEL_0 = (0),
    PIT_CHANNEL_1 = (1),
    PIT_CHANNEL_2 = (2)
} PIT_CHANNEL;

/*!
 * Specifies the operating mode of a channel.
 */
typedef enum {
    PIT_MODE_0 = (0b000 << 1),
    PIT_MODE_1 = (0b001 << 1),
    PIT_MODE_2 = (0b010 << 1),
    PIT_MODE_3 = (0b011 << 1),
    PIT_MODE_4 = (0b100 << 1),
    PIT_MODE_5 = (0b101 << 1),
} PIT_MODE;

/*!
 * Sets channel \p channel up to work in operating mode \p mode with frequency
 * divider \p freq_div.
 * \param channel Channel which is to be setup.
 * \param mode Desired operating mode.
 * \param freq_div Desired frequency divider.
 */
void pit_setup_channel(PIT_CHANNEL channel, PIT_MODE mode, uint16_t freq_div);

#endif