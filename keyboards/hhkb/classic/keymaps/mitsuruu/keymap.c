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

#include QMK_KEYBOARD_H
#include "mitsuruu.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_60_hhkb(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_NUBS, KC_GRV,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, HH_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, FN,
                 KC_LGUI, LM_LALT,                            KC_SPC,                             FN,      KC_RGUI
    ),
    [_FN] = LAYOUT_60_hhkb(
        KC_PWR,  EXT_F1,  EXT_F2,  EXT_F3,  EXT_F4,  EXT_F5,  EXT_F6,  EXT_F7,  EXT_F8,  EXT_F9,  EXT_F10, EXT_F11, EXT_F12, KC_NUHS, KC_DEL,
        KC_CAPS, _______, _______, _______, _______, _______, _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, KC_UP,   _______, _______,
        _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT,          KC_PENT,
        _______,          KC_MPRV, KC_MNXT, _______, _______, _______, KC_PPLS, KC_PMNS, KC_END,  KC_PGDN, KC_DOWN, _______, _______,
                 _______, _______,                            KC_MPLY,                            _______, _______
    ),
    [_LM] = LAYOUT_60_hhkb(
        _______, _______, _______, _______, KC_F4,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                 _______, _______,                            _______,                            _______, _______
    )
};
// clang-format on

bool dip_switch_update_mask_keymap(uint32_t state) {
    eeconfig_read_keymap(&keymap_config);

    /* ALT and GUI are swapped in keymap to allow the LM_LALT macro to work
     * so we need to invert SW5 here so that it still functions the same without
     * breaking forcing LM_LALT to act as a regular KC_LALT.
     */
    keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = !keymap_config.swap_lalt_lgui;

    eeconfig_update_keymap(&keymap_config);
    clear_keyboard();
    return true;
}
