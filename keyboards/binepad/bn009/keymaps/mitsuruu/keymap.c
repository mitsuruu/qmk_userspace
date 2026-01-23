#include QMK_KEYBOARD_H
#include "mitsuruu.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ortho_3x3(
        KC_7,    KC_8,    KC_9,
        KC_4,    KC_5,    KC_6,
        KC_1,    KC_2,    FN
    ),
    [_FN] = LAYOUT_ortho_3x3(
        QK_BOOT, KC_KP_8, KC_KP_9,
        KC_KP_4, KC_KP_5, KC_KP_6,
        KC_KP_1, KC_KP_2, _______
    )
};
