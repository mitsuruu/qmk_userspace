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

#pragma once

#include "quantum.h"
// clang-format off
#define FN      MO(_FN)
#define FN_ALT  MO(_FN_ALT)
#define LM_LALT LM(_LM, MOD_LALT)

/* Split layer definitions (mainly for Lily58) */
#define _RAISE  _FN
#define _LOWER  _FN_ALT
#define RAISE   MO(_RAISE)
#define LOWER   MO(_LOWER)
#define ADJUST  MO(_ADJUST)
// clang-format on

#if defined(HHKB_ENABLE)
#    include "hhkb/hhkb.h"
#    define HH_BSPC HHKB_BACKSPACE
#endif

#if defined(SECRETS_ENABLE)
#    include "secrets/secrets.h"
#endif

enum layers { _BASE = 0, _FN, _FN_ALT, _ADJUST, _LM };

enum userspace_keycodes {
    EXT_F1 = SAFE_RANGE,
    EXT_F2,
    EXT_F3,
    EXT_F4,
    EXT_F5,
    EXT_F6,
    EXT_F7,
    EXT_F8,
    EXT_F9,
    EXT_F10,
    EXT_F11,
    EXT_F12,
    SECRET1,
    SECRET2,
    SECRET3,
    SECRET4,
    SECRET5,
#if defined(HHKB_ENABLE)
    HHKB_BACKSPACE,
#endif
    NEW_SAFE_RANGE
};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record);

bool dip_switch_update_mask_keymap(uint32_t state);
