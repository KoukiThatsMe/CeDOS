/*!
 * Driver for speakers.
 */
#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>

/*!
 * Interface for a speaker driver.
 */
typedef struct {
    /*!
     * Initializes the speaker.
     * \return 1 on success, 0 on failure
     */
    int (*init)(void);

    /*!
     * Sets the current frequency of the speaker.
     * \param freq Frequency to by output by the speaker.
     * \param volume Volume of the sound.
     * \return 1 on success, 0 on failure.
     */
    int (*make_sound)(uint32_t freq, uint32_t volume);

    /*!
     * Mutes the speaker.
     * \return 1 on success, 0 on failure.
     */
    int (*mute)();
} SPEAKER_DRIVER;

//! Driver for the bios speaker.
SPEAKER_DRIVER bios_sp;

#endif