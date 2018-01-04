#include "cedos/drivers/speaker.h"
#include "cedos/pit.h"
#include "assembly.h"

#include <stdint.h>

/*!
 * Initializes the bios speaker.
 * \return 1 on success, 0 on failure
 */
int bios_sp_init(void);

/*!
 * Sets the current frequency of the bios speaker.
 * \param freq Frequency to by output by the bios speaker.
 * \param volume Volume of the sound.
 * \return 1 on success, 0 on failure.
 */
int bios_sp_make_sound(uint32_t freq, uint32_t volume);

/*!
 * Mutes the bios speaker.
 * \return 1 on success, 0 on failure.
 */
int bios_sp_mute();

//! Driver for the bios speaker.
SPEAKER_DRIVER bios_sp = {
    bios_sp_init,
    bios_sp_make_sound,
    bios_sp_mute
};

int bios_sp_init(void) {
    bios_sp_mute();
    return 1;
}

int bios_sp_make_sound(uint32_t freq, uint32_t volume) {
    /**
     * volume for the bios speaker:
     *  - 0: mute
     *  - other: full volume
     */

    if (volume) {
        pit_setup_channel(PIT_CHANNEL_2, PIT_MODE_3, (uint16_t)(1193180 / freq));
        outb(inb(0x61) | 0b00000011, 0x61);
    } else {
        bios_sp_mute();
    }

    return 1;
}

int bios_sp_mute() {
    outb(inb(0x61) & (~0b00000011), 0x61);
    return 1;
}