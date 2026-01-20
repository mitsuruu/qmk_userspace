# HHKB Pro Classic (PD-KB401) Userspace

### Base Layout

![HHKB base layout](https://github.com/mitsuruu/hhkb_qmk_resources/blob/main/img/base-layer.png?raw=true)

### FN Layout

![HHKB FN layout](https://github.com/mitsuruu/hhkb_qmk_resources/blob/main/img/fn-layer.png?raw=true)

## DIP Switch Configuration

Alike with the stock HHKB keyboard, the first two DIP Switches (SW1 and SW2) can
be used to configure the "mode" of the board. The DIP switches can be configured
in the combinations below. [1]

| SW1 | SW2 |  MODE   |
| :-- | :-- | :-----: |
| OFF | OFF |  HHKB   |
| ON  | OFF | WINDOWS |
| OFF | ON  |   MAC   |
| ON  | ON  |  HHKB   |

In the case of having SW1 and SW2 both ON will result in HHKB mode, as "sun
mode" is not supported on the HHKB Pro Classic. [2] Additionally, some of the
DIP switches are supported in the userspace, however not all of them are
supported in their original configurations.

| DIP Switch | Function                          | Supported? |
| :--------: | :-------------------------------- | :--------: |
|    SW1     | Layout switch 1                   |    YES     |
|    SW2     | Layout switch 2                   |    YES     |
|    SW3     | Delete key becomes Backspace (on) |    YES     |
|    SW4     | Left ◇ key becomes Fn (on)        |     NO     |
|    SW5     | Swap ◇ and Alt (◇->Alt, Alt->◇)   |    YES     |
|    SW6     | Power Savings Mode                |    NO*     |

*SW6 acts as a bootloader toggle switch when using the dapboot bootloader. While
it's functionality can be included, it should be ignored to prevent conflicts.

### Overriding DIP switch defaults

In order to override the DIP switches functionality, modify the
`dip_switch_update_mask_user(uint32_t state)` and
`process_record_hhkb(uint16_t keycode, keyrecord_t *record)` functions in
`hhkb.c`. As an example, overriding SW4 to make the Escape key send the string
`HHKB is my favourite!`, you could add the following into
`process_record_hhkb(uint16_t keycode, keyrecord_t *record)`

```c
case KC_ESCAPE:
    // SW4 - Replace ESC with text
    if (hhkb_dip_switch_config.sw4) {
        if (record->event.pressed) {
            send_string("HHKB is my favourite!");
            return false;
        }
    }
    return true;
```

# Sources

[[1] HHKB Professional Classic (US Layout) User's Guide](https://origin.pfultd.com/downloads/hhkb/manual/P3PC-6661-01EN.pdf)

[[2] HHKB Pro @ Deskthority](https://deskthority.net/wiki/Talk:HHKB_Professional#Sun_mode_on_HHKB_Professional_2)

# License

Copyright 2026 Mitsuruu @mitsuruu

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.
