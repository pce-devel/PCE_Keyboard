# Tsuushin Keyboard

The unreleased PC Engine Keyboard

## Technical Overview

The Tsuushin Keyboard is capable of transmitting the state of up to 112 keys.
It does this by acting as a set of controllers via multiap, sending a start byte, followed
by 16 bytes of data (where only 7 bits are encoded in each), and an end byte, in the same
way as a giant-sized multiap might present 18 controllers to the host.

## Encoding Method

The similarities to the PC-88 keyboard don't end at the style and layout; the keypress information
is represented in nearly an identical format (except for the 7-bits per byte).

| Byte in Sequence | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Comment |
+------------------+-------+-------+-------+-------+-------+-------+-------+-------+---------|
| Byte #0          | '0' | '0' | '0' | '0' | '0' | '0' | '1' | '0' | Start Byte - Literal bit values |
| Byte #1          | '1' | KyPd 6 | KyPd 5 | KyPd 4 | KyPd 3 | KyPd 2 | KyPd 1 | Kypd 0 | Data - '0' value when key pressed; '1' value otherwise |
| Byte #2          | '1' | KyPd . | KyPd , | KyPd = | KyPd + | KyPd * | KyPd 9 | Kypd 8 | |
| Byte #3          | '1' | F | E | D | C | B | A | @ | |
| Byte #4          | '1' | N | M | L | K | J | I | H | |
| Byte #5          | '1' | V | U | T | S | R | Q | P | |
| Byte #6          | '1' | ^ | ] | Yen Sign | [ | Z | Y | X | |
| Byte #17         | '0' | '0' | '0' | '0' | '0' | '0' | '1' | '0' | End Byte |

## Emulation

### Use of Macropad

Teso program for PCE Keyboard
