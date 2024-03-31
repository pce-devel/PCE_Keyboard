# Tsuushin Keyboard - Technical

A technical overviw of the unreleased Tsuushin Keyboard

## Technical Overview

The Tsuushin Keyboard is capable of transmitting the state of up to 112 keys.
It does this by acting as a set of controllers via multiap, sending a start byte, followed
by 16 bytes of data (where only 7 bits are encoded in each), and an end byte, in the same
way as a giant-sized multiap might present 18 controllers to the host.

## Encoding Method

The similarities to the PC-88 keyboard don't end at the style and layout; the keypress information
is represented in nearly an identical format (except for the 7-bits per byte).

Note that the "(Not In Use)" keys may correspond to a key, but these are currently no known or understood.


| Byte in Sequence | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Comment |
|------------------|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|---------|
| Byte #0          | '0' | '0' | '0' | '0' | '0' | '0' | '1' | '0' | Start Byte - Literal bit values (0x02) |
| Byte #1          | '1' | NumPad 6 | NumPad 5 | NumPad 4 | NumPad 3 | NumPad 2 | NumPad 1 | NumPad 0 | Data - '0' value when key pressed; '1' value otherwise |
| Byte #2          | '1' | NumPad . | NumPad , | NumPad = | NumPad + | NumPad * | NumPad 9 | NumPad 8 | |
| Byte #3          | '1' | F | E | D | C | B | A | @ | |
| Byte #4          | '1' | N | M | L | K | J | I | H | |
| Byte #5          | '1' | V | U | T | S | R | Q | P | |
| Byte #6          | '1' | ^ | ] | Yen Sign | \[ | Z | Y | X | |
| Byte #7          | '1' | 6 | 5 | 4 | 3 | 2 | 1 | 0 | |
| Byte #8          | '1' | / | . | , | ; | : | 9 | 8 | |
| Byte #9          | '1' | (Not Used ?) | KANA | GRPH | (Not Used ?) | Right Arrow | Up Arrow | CLEAR | |
| Byte #10         | '1' | Space | F5 | F4 | F3 | F2 | F1 | STOP | |
| Byte #11         | '1' | NumPad / | NumPad - | COPY | HELP | Left Arrow | Down Arrow | Tab | |
| Byte #12         | '1' | G | _ (underscore) | O | (Not Used ?) | (Not Used ?) | Roll Up | Roll Down | |
| Byte #13         | '1' | Insert | Backspace | F10 | F9 | F8 | F7 | F6 | |
| Byte #14         | '1' | W | NumPad 7 | Control | ZENKAKU (Width) | PC | KETTEI (unconvert) | HENKOU (convert) |  |
| Byte #15         | '1' | Escape | Delete | Caps Lock | Right Shift | Left Shift | NumPad Enter | Return |  |
| Byte #16         | '1' | 7 | - (Minus) | (Not Used ?) | (Not Used ?) | (Not Used ?) | (Not Used ?) | (Not Used ?) |  |
| Byte #17         | '0' | '0' | '0' | '0' | '0' | '0' | '1' | '0' | End Byte (0x02) |

For comparison, Joypad keys are as follows:

| Byte in Sequence | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Comment |
|------------------|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|---------|
| Joypad Values    | Left | Down | Right | Up | Run | Select | II | I | Top 4 bits cannot be all zeroes on regular joypad | 


### Comparison to PC-88 Encoding

The PC-88 encoding pattern above can be compared against the PC-88 pattern found here:
[https://www.leadedsolder.com/2018/04/14/pc88-keyboard-follies.html](https://www.leadedsolder.com/2018/04/14/pc88-keyboard-follies.html)


