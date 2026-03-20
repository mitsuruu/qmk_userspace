/* Copyright 2026 Mitsuruu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mitsuruu.h"
#include "quantum.h"

// Keymap-specific process_record_user()
__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

__attribute__((weak)) bool dip_switch_update_mask_keymap(uint32_t state) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keymap(keycode, record)) {
        return false;
    }

#if defined(HHKB_ENABLE)
    if (!process_record_hhkb(keycode, record)) {
        return false;
    }
#endif

#if defined(OS_DETECTION_ENABLE)
    if (!process_record_os_detection(keycode, record)) {
        return false;
    }
#endif

    switch (keycode) {
        case EXT_F1 ... EXT_F12:
            if (!record->event.pressed) {
                uint8_t temp_mod = get_mods();
                uint8_t temp_osm = get_oneshot_mods();
                clear_mods();
                clear_oneshot_mods();
                tap_code(((temp_mod | temp_osm) & MOD_MASK_SHIFT ? KC_F13 : KC_F1) + keycode - EXT_F1);
                set_mods(temp_mod);
            }
            return false;
        case KC_ESCAPE:
            if (record->event.pressed) {
                if ((get_mods() & MOD_MASK_SHIFT) == MOD_MASK_SHIFT) {
                    clear_mods();
                    reset_keyboard();
                    return false;
                }
            }
            return true;
    }

    return true;
}
