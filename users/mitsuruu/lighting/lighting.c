#include "mitsuruu.h"

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _FN:
            rgblight_setrgb(0x00, 255, 255);
            break;
        case _FN_ALT:
            rgblight_setrgb(255, 255, 0x00);
        case _ADJUST:
            rgblight_setrgb(255, 0x00, 255);
        default: //  for any other layers, or the default layer
            rgblight_setrgb(0x00, 0x00, 0x00);
            break;
    }
    return state;
}
