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
            break;
    }

    eeconfig_update_keymap(&keymap_config);
    clear_keyboard();

    return true;
}
