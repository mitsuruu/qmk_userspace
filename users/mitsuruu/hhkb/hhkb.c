/* Copyright 2023 Mitsuruu
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

#include "quantum.h"
#include "hhkb.h"
#include "mitsuruu.h"

#if OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

bool dip_switch_update_mask_user(uint32_t state) {
    eeconfig_read_keymap(&keymap_config);

    hhkb_dip_switch_config.layout = state & (3UL << 0);
    hhkb_dip_switch_config.sw3    = state & (1UL << 2);
    hhkb_dip_switch_config.sw4    = state & (1UL << 3);
    hhkb_dip_switch_config.sw5    = state & (1UL << 4);
    hhkb_dip_switch_config.sw6    = state & (1UL << 5);

    if (hhkb_dip_switch_config.layout == INVALID) {
        hhkb_dip_switch_config.layout = HHKB;
    }

    // SW5 - Swap GUI and ALT keys -- This option is ignored when running in Mac mode.
    if (hhkb_dip_switch_config.layout != MAC) {
        keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = hhkb_dip_switch_config.sw5;
    }

    eeconfig_update_keymap(&keymap_config);

    return dip_switch_update_mask_keymap(state);
}

bool process_record_hhkb(uint16_t keycode, keyrecord_t *record) {
#if OS_DETECTION_ENABLE
    os_variant_t current_os = detected_host_os();
#endif
    switch (keycode) {
        case HHKB_BACKSPACE:
            // SW3 - Swap backspace and delete
            if (hhkb_dip_switch_config.sw3) {
                if (record->event.pressed) {
                    register_code(KC_BACKSPACE);
                } else {
                    unregister_code(KC_BACKSPACE);
                }
            } else {
                if (record->event.pressed) {
                    register_code(KC_DELETE);
                } else {
                    unregister_code(KC_DELETE);
                }
            }
            return false;

        case KC_SLEP:
#if OS_DETECTION_ENABLE
            if (current_os == OS_MACOS || current_os == OS_IOS) {
                return true;
            }
#endif
            if (hhkb_dip_switch_config.layout != MAC) {
                uint16_t fallthrough = keymap_key_to_keycode(biton32(default_layer_state), record->event.key);
                if (record->event.pressed) {
                    register_code(fallthrough);
                    return false;
                } else {
                    unregister_code(fallthrough);
                    return false;
                }
            }
            break;
    }
    return true;
}
