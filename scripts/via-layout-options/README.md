# via-layout-options

Generates the `VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT` value for a keyboard, so VIA
starts with the right layout options (split backspace, bottom row, etc.) selected
after a fresh flash or EEPROM reset.

## Requirements

- Python 3 (standard library only)
- The `qmk` CLI, set up with a `QMK_HOME` (`qmk env QMK_HOME` should print it)
- Network access to `usevia.app`, unless you pass a local definition with `--json`

## Usage

```sh
via-layout-options.py <keyboard> [value ...] [--json <file>]
```

| Argument        | Description                                                                                      |
| --------------- | ------------------------------------------------------------------------------------------------ |
| `keyboard`      | Keyboard path as used by QMK, e.g. `cipulot/ec_60x`                                              |
| `value ...`     | Choice number for each layout option, in the order VIA lists them. Any left out are prompted for |
| `--json <file>` | Use a local VIA definition instead of downloading one                                            |

### Interactively

Run it with just the keyboard to be asked for each option, with its choices
listed:

```console
$ scripts/via-layout-options/via-layout-options.py cipulot/ec_60x

Split Backspace:
  0: Off
  1: On
> 1

Split Left Shift:
  0: Off
  1: On
> 0

Right Shift:
  0: Unified
  1: 1.75U | 1U
  2: 1U | 1.75U
> 1
...
```

Running it this way once is also the easiest way to find out the option order
and what each number means for a keyboard.

### With values

Pass a choice number for each option, in order:

```console
$ scripts/via-layout-options/via-layout-options.py cipulot/ec_60x 1 0 1 0 8

// Split Backspace: On
// Split Left Shift: Off
// Right Shift: 1.75U | 1U
// ISO: Off
// Bottom Row: 6U HHKB
#define VIA_EEPROM_LAYOUT_OPTIONS_SIZE 2
#define VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT 0x128
```

If you pass fewer values than there are options, the remaining ones are
prompted for.

### Using the output

Copy the `#define` lines into your keymap's `config.h`, e.g.
`keyboards/cipulot/ec_60x/keymaps/mitsuruu/config.h`. The comment lines are
there so you can see what the value means later, and can be kept too.

QMK only writes the default when it resets the VIA EEPROM, which normally
happens when you flash a new build. If the old layout options are still
selected after flashing, clear the EEPROM with `EE_CLR`.

## How it works

### 1. Finding the VIA definition

VIA definitions in [the-via/keyboards](https://github.com/the-via/keyboards)
aren't always stored under the same path as the keyboard in QMK (for example
`gray_studio/space65r3` is `v3/graystudio/space65/space65-r3.json`), so the
script doesn't use paths. Instead it does the same lookup the VIA app does when
a keyboard is plugged in:

1. Run `qmk info -kb <keyboard> -f json` to get the keyboard's USB vendor ID
   and product ID. This includes values the keyboard inherits from its parent
   folders.
2. Download `https://usevia.app/definitions/v3/<id>.json`, where
   `<id> = (vendorId << 16) | productId` written as a decimal number.

Two quirks of `usevia.app` are handled:

- It rejects Python's default User-Agent with a 403, so the script sends its own.
- An unknown ID returns the VIA web app's HTML page with a 200 status instead
  of a 404, so a response that isn't JSON is treated as "no definition".

If the keyboard has no VIA definition, or its USB IDs in QMK don't match the
published definition, download or write the definition yourself and pass it
with `--json`.

### 2. Reading the layout options

The options come from `layouts.labels` in the definition. Each entry is either:

- a string, e.g. `"Split Backspace"`: an on/off option with choices
  `0: Off` and `1: On`
- a list, e.g. `["Right Shift", "Unified", "1.75U | 1U", "1U | 1.75U"]`: the
  first item is the option name and the rest are its choices, numbered from 0

### 3. Packing the value

VIA doesn't give each option a hex digit. It packs them into bit fields, the
same way the VIA app's `src/utils/bit-pack.ts` does:

- Each option uses the fewest bits that fit its choices: 1 bit for 2 choices,
  2 bits for 3–4, 3 bits for 5–8, 4 bits for 9–16, and so on.
- The **first** option goes in the highest bits and the **last** option in the
  lowest bits.

For `cipulot/ec_60x` that gives 9 bits:

| Bits | Option           | Choices |
| ---- | ---------------- | ------- |
| 8    | Split Backspace  | 2       |
| 7    | Split Left Shift | 2       |
| 6–5  | Right Shift      | 3       |
| 4    | ISO              | 2       |
| 3–0  | Bottom Row       | 13      |

so the value is:

```
(splitBackspace << 8) | (splitLeftShift << 7) | (rightShift << 5) | (iso << 4) | bottomRow
```

With `1 0 1 0 8` that's `0x100 | 0x20 | 0x08 = 0x128`.

### 4. Checking the storage size

QMK stores the layout options in `VIA_EEPROM_LAYOUT_OPTIONS_SIZE` bytes, which
defaults to 1 (8 bits) in `quantum/via.h`. Any bits above that are silently
dropped, so on the ec_60x a 1-byte setting loses Split Backspace entirely.

The script reads the current size from `quantum/via.h` and from each
`config.h` on the keyboard's path under `QMK_HOME/keyboards`. If the options
need more bytes than that, it also prints a `VIA_EEPROM_LAYOUT_OPTIONS_SIZE`
define with the size needed.

It doesn't read your keymap's `config.h`, so it will keep printing the `SIZE`
line even after you've added it there. That's harmless; just don't add it
twice.

## Limitations

- Only definitions published to VIA (or passed with `--json`) can be used.
- Choice numbers are positional, so you need to know the option order. Run it
  without values to see it.
- The size check only looks for a plain `#define` in `config.h` files, not
  values set in other headers or passed as compiler flags. It also doesn't
  understand `#ifdef`s, so if there's more than one `#define` it uses the last
  one it finds.
