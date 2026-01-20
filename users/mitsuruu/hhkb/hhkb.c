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
    // NOTE: This dipswitch breaks support for LM(LAYER, MOD_LALT) keybinds unless you
    //       pre-swap them in the keymap, then invert it in dip_switch_update_mask_keymap().
    keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = hhkb_dip_switch_config.sw5;
    if (hhkb_dip_switch_config.layout == MAC) {
        keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = false;
    }

    eeconfig_update_keymap(&keymap_config);
    clear_keyboard();

    return dip_switch_update_mask_keymap(state);
}

bool process_record_hhkb(uint16_t keycode, keyrecord_t *record) {
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

        // Layout differences
        case KC_PWR:
            if (hhkb_dip_switch_config.layout != MAC) {
                if (record->event.pressed) {
                    register_code(KC_TRNS);
                } else {
                    unregister_code(KC_TRNS);
                }
                return false;
            }
            return true;

        // NUBS and Grave are swapped around on MacOS for some reason (thanks Apple)
        // So we pre-swap them here, so that MacOS swaps them back for us...
        // Unless you're using an ANSI layout on MacOS in which case Grave works fine,
        // and NUBS needs to be remapped as NUHS in order to be a Backslash... (once again, thanks Apple)
        // And unfortunately, we can't use SW6 to figure out which to use because SW6 is used by Dapboot.
        // So for now, this works under the assumption that the layout in MacOS will be US not GB.
        // In order to change it to GB, swap the KC_NUHS for KC_GRV; then uncomment the second `case` statement.
        case KC_NUBS:
            if (hhkb_dip_switch_config.layout != MAC) {
                return true;
            }

            if (record->event.pressed) {
                register_code(KC_NUHS);
            } else {
                unregister_code(KC_NUHS);
            }
            return false;
            // case KC_GRV:
            //     if (hhkb_dip_switch_config.layout != MAC) {
            //         return true;
            //     }

            //     if (record->event.pressed) {
            //         register_code(KC_NUBS);
            //     } else {
            //         unregister_code(KC_NUBS);
            //     }
            //     return false;
    }
    return true;
}
