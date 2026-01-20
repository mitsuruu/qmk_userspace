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

bool process_record_hhkb(uint16_t keycode, keyrecord_t *record);

typedef union {
    struct {
        // This layout `uint8_t` is actually a 2-bit unsigned int which combines sw1 and sw2.
        // See the layouts enum for further details
        // Layouts:
        // [SW1: 0, SW2: 0] - HHKB
        // [SW1: 0, SW2: 1] - WINDOWS
        // [SW1: 1, SW2: 0] - MAC
        // [SW1: 1, SW2: 1] - INVALID (gets set back to HHKB)
        uint8_t layout : 2;
        // SW3: Swap Backspace and Delete.
        // On: Delete = 'BS'
        // Off: Delete = 'DEL'
        bool sw3 : 1;
        // SW4: Make Left ◇ a Fn key.
        // On: Left ◇ = 'Fn'
        // Off: Left ◇ = '◇'
        bool sw4 : 1;
        // SW5: Swap Alt and Gui keys.
        // On: ◇ = 'Alt', Alt = '◇'
        // Off: ◇ = '◇', Alt = 'Alt'
        bool sw5 : 1;
        // If possible, avoid using SW6 as it is used by dapboot for entering the bootloader on boot.
        bool sw6 : 1;
    };
} hhkb_dip_switch_config_t;

hhkb_dip_switch_config_t hhkb_dip_switch_config;

enum layouts { HHKB = 0, WINDOWS = 1, MAC = 2, INVALID = 3 };
