# SPDX-FileCopyrightText: 2021 Emma Humphries for Adafruit Industries
#
# SPDX-License-Identifier: MIT

# MACROPAD Hotkeys example: Firefox web browser for Linux

from adafruit_hid.keycode import Keycode # REQUIRED if using Keycode.* values

app = {                       # REQUIRED dict, must be named 'app'
    'name' : 'Tsushin Keyboard', # Application name
    'macros' : [              # List of button macros...
        # COLOR    LABEL    KEY SEQUENCE
        # 1st row ----------
        (0x181818, '@', [53]),
        (0x181818, '^', [46]),
        (0x181818, '_', [100]),
        # 2nd row ----------
        (0x001800, 'Stop', [72]),
        (0x001800, 'Copy', [70]),
        (0x000000, '', []),
        # 3rd row ----------
        (0x000020, 'Kana', [68]),
        (0x000020, 'GRPH', [71]),
        (0x000020, 'PC', [69]),
        # 4th row ----------
        (0x000020, 'Kettei', [226]),
        (0x000020, 'Henkou', [230]),
        (0x000020, 'Hankaku', [228]),
        # Encoder button ---
        (0x000000, '', []) # 
    ]
}
