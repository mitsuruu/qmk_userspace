/* Copyright 2024 Cipulot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include "keycodes.h"
#include "socd_cleaner.h"
#include QMK_KEYBOARD_H
#include "config.h"
#include "ec_switch_matrix.h"
#include "mitsuruu.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    [_BASE] = LAYOUT_all(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_NUBS,   KC_GRV,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSPC,   KC_NO,
        KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NO,    KC_ENTER,
        KC_LSFT,  KC_NO,    KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_RSFT,   FN,
        KC_NO,    KC_LGUI,  LM_LALT,            KC_SPC,                  KC_SPC,                  KC_SPC,             FN,       KC_RGUI,  KC_NO,     KC_NO),

    [_FN] = LAYOUT_all(
        QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_NUHS,  KC_DEL,
        KC_CAPS,  SOCDON,   SOCDOFF,  _______,  _______,  _______,  _______,  KC_INS,   KC_PSCR,  KC_SCRL,  KC_PAUS,  KC_UP,    _______,  NK_TOGG,  _______,
        _______,  KC_VOLD,  KC_VOLU,  KC_MUTE,  _______,  _______,  KC_PAST,  KC_PSLS,  KC_HOME,  KC_PGUP,  KC_LEFT,  KC_RIGHT, _______,  KC_PENT,
        _______,  _______,  KC_MPRV,  KC_MNXT,  _______,  _______,  _______,  KC_PPLS,  KC_PMNS,  KC_END,   KC_PGDN,  KC_DOWN,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,                 KC_MPLY,                 _______,            _______,  _______,  _______,  _______),

    [_LM] = LAYOUT_all(
        _______,  _______,  _______,  _______,  KC_F4,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,            _______,                 _______,                 _______,            _______,  _______,  _______,  _______)
    // clang-format on
};

socd_cleaner_t socd_opposing_pairs[] = {
    {{KC_W, KC_S}, SOCD_CLEANER_LAST},
    {{KC_A, KC_D}, SOCD_CLEANER_LAST},
};

static const uint16_t default_bottoming_reading[MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    { 408,  403,  530,  586,  486,  531,  524,  562,  519,  600,  546,  531,  511,  410,  455},
    { 494,  468,  521,  574,  500,  547,  479,  562,  582,  582,  492,  552,  489,  315, 1023},
    { 326,  519,  546,  454,  502,  602,  536,  624,  601,  612,  581,  588, 1023,  325, 1023},
    { 347, 1023,  490,  590,  550,  562,  538,  585,  593,  584,  569,  497,  339, 1023,  388},
    {1023,  439,  419, 1023, 1023, 1023,  328, 1023, 1023, 1023,  410,  343, 1023, 1023, 1023},
    // clang-format on
};

void eeconfig_init_user(void) {
    memcpy(eeprom_ec_config.bottoming_reading, default_bottoming_reading, sizeof(default_bottoming_reading));
    eeconfig_update_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);
}
