#!/usr/bin/env python3
"""Generate VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT for a keyboard's VIA layout options.

Usage:
    via-layout-options <keyboard> [value ...]
    via-layout-options cipulot/ec_60x 1 0 1 0 8

Values are given in the order of the VIA definition's `layouts.labels`; any that
are omitted are prompted for. The VIA definition is fetched from usevia.app by the
keyboard's USB vendor/product ID (the same lookup VIA does) unless --json points
at a local file.
"""

import argparse
import json
import math
import re
import subprocess
import sys
import urllib.error
import urllib.request
from pathlib import Path

VIA_DEFINITIONS_URL = "https://usevia.app/definitions/v3"


def qmk_home():
    return Path(subprocess.check_output(["qmk", "env", "QMK_HOME"], text=True).strip())


def qmk_info(keyboard):
    result = subprocess.run(
        ["qmk", "info", "-kb", keyboard, "-f", "json"], capture_output=True, text=True
    )
    if result.returncode != 0:
        sys.exit(f"error: qmk info failed for {keyboard}:\n{result.stderr.strip()}")
    return json.loads(result.stdout)


def load_via_definition(keyboard, info, json_path):
    if json_path:
        return json.loads(Path(json_path).read_text())

    usb = info.get("usb", {})
    if "vid" not in usb or "pid" not in usb:
        sys.exit(f"error: {keyboard} has no USB VID/PID, pass a definition with --json")

    # VIA keys its definitions by (vendorId << 16) | productId
    vid, pid = int(usb["vid"], 16), int(usb["pid"], 16)
    url = f"{VIA_DEFINITIONS_URL}/{(vid << 16) | pid}.json"
    # usevia.app rejects urllib's default User-Agent with a 403
    request = urllib.request.Request(url, headers={"User-Agent": "via-layout-options"})
    try:
        with urllib.request.urlopen(request) as res:
            return json.load(res)
    except urllib.error.HTTPError as e:
        if e.code != 404:
            sys.exit(f"error: fetching {url} failed: HTTP {e.code}")
    except json.JSONDecodeError:
        # Unknown IDs get the app's HTML page with a 200 rather than a 404
        pass
    sys.exit(
        f"error: no VIA definition for {keyboard} "
        f"(VID 0x{vid:04X}, PID 0x{pid:04X}), pass one with --json"
    )


def find_define(name, files):
    """Return the value of the last `#define name value` found in files."""
    value = None
    for file in files:
        if file.is_file():
            match = re.search(rf"^\s*#\s*define\s+{name}\s+(\S+)", file.read_text(), re.M)
            if match:
                value = match.group(1)
    return value


def options_size(home, keyboard):
    """VIA_EEPROM_LAYOUT_OPTIONS_SIZE as QMK would see it for this keyboard."""
    files = [home / "quantum" / "via.h"]
    parts = Path(keyboard).parts
    for i in range(1, len(parts) + 1):
        files.append(home / "keyboards" / Path(*parts[:i]) / "config.h")
    return int(find_define("VIA_EEPROM_LAYOUT_OPTIONS_SIZE", files) or 1, 0)


def parse_labels(labels):
    """Return a list of (name, choices) in VIA label order."""
    options = []
    for label in labels:
        if isinstance(label, list):
            options.append((label[0], label[1:]))
        else:
            options.append((label, ["Off", "On"]))
    return options


def bit_size(num_choices):
    return max(1, math.ceil(math.log2(num_choices)))


def prompt(name, choices):
    print(f"\n{name}:")
    for i, choice in enumerate(choices):
        print(f"  {i}: {choice}")
    while True:
        answer = input("> ").strip()
        if answer.isdigit() and int(answer) < len(choices):
            return int(answer)
        print(f"  enter a number from 0 to {len(choices) - 1}")


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("keyboard", help="keyboard path, e.g. cipulot/ec_60x")
    parser.add_argument("values", nargs="*", type=int, help="choice index per option")
    parser.add_argument("--json", help="local VIA definition instead of fetching it")
    args = parser.parse_args()

    home = qmk_home()
    info = qmk_info(args.keyboard)
    keyboard = args.keyboard

    definition = load_via_definition(keyboard, info, args.json)
    options = parse_labels(definition.get("layouts", {}).get("labels", []))
    if not options:
        sys.exit(f"error: {keyboard} has no VIA layout options")
    if len(args.values) > len(options):
        sys.exit(f"error: got {len(args.values)} values for {len(options)} options")

    selected = []
    for i, (name, choices) in enumerate(options):
        if i < len(args.values):
            if not 0 <= args.values[i] < len(choices):
                sys.exit(f"error: {name} must be 0-{len(choices) - 1}, got {args.values[i]}")
            selected.append(args.values[i])
        else:
            selected.append(prompt(name, choices))

    # VIA packs options MSB-first in label order, so the last option is in the lowest bits
    value = 0
    for (name, choices), choice in zip(options, selected):
        value = (value << bit_size(len(choices))) | choice

    total_bits = sum(bit_size(len(choices)) for _, choices in options)
    needed_bytes = max(1, math.ceil(total_bits / 8))

    print()
    for (name, choices), choice in zip(options, selected):
        print(f"// {name}: {choices[choice]}")
    if needed_bytes > options_size(home, keyboard):
        print(f"#define VIA_EEPROM_LAYOUT_OPTIONS_SIZE {needed_bytes}")
    print(f"#define VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT 0x{value:X}")


if __name__ == "__main__":
    main()
