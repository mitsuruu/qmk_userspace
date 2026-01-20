#include "secrets.h"

__attribute__((weak)) bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SECRET1 ... SECRET5:
            if (record->event.pressed) {
                send_string(secret[keycode - SECRET1]);
            }
            return false;
    }
}
