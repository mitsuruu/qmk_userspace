#include QMK_KEYBOARD_H
#include "mitsuruu.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ortho_3x3(
        KC_7,    KC_8,    KC_9,
        KC_4,    KC_5,    KC_6,
        KC_1,    KC_2,    FN
    ),
    [_FN] = LAYOUT_ortho_3x3(
        KC_F18,  KC_F19,  KC_F20,
        KC_F15,  KC_F16,  KC_F17,
        KC_F13,  KC_F14,  _______
    )
};
// clang-format on
