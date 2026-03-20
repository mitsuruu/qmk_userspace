#include "mitsuruu.h"
#include "quantum.h"

__attribute__((weak)) bool process_detected_host_os_keymap(os_variant_t detected_os) {
    return true;
}

bool process_detected_host_os_user(os_variant_t detected_os) {
    if (!process_detected_host_os_keymap(detected_os)) {
        return false;
    }

    eeconfig_read_keymap(&keymap_config);

    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            keymap_config.swap_lalt_lgui = true;
            keymap_config.swap_ralt_rgui = true;
            break;
        case OS_WINDOWS:
        case OS_LINUX:
        case OS_UNSURE:
            keymap_config.swap_lalt_lgui = false;
            keymap_config.swap_ralt_rgui = false;
            break;
    }

    eeconfig_update_keymap(&keymap_config);

    return true;
}

bool process_record_os_detection(uint16_t keycode, keyrecord_t *record) {
    os_variant_t detected_os = detected_host_os();

    switch (keycode) {
        // Force override detected OS, useful for wireless/BT boards.
        case KC_OS_MACOS:
            process_detected_host_os_user(OS_MACOS);
            return false;
        case KC_OS_WINDOWS:
            process_detected_host_os_user(OS_WINDOWS);
            return false;
        case KC_NUBS: {
            if (detected_os == OS_MACOS || detected_os == OS_IOS) {
                if (record->event.pressed) {
                    register_code(KC_BSLS);
                } else {
                    unregister_code(KC_BSLS);
                }
                return false;
            }
            return true;
        }
    }

    return true;
}
