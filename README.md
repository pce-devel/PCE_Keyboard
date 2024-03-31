# Tsuushin Keyboard

Temporary location for PCE keyboard

## History of Tsuushin Keyboard

In 1989/1990, NEC and Hudson were working on an expansion device for the PC Engine called
the Tshusin Booster, which would bring modem capabilities to the PC Engine. This device was
never brought to market, and was cancelled in 1990. Very few units were ever produced, so it
is unlikely that you would be able to locate one these days, at any price.

The software component of the Tshushin Booster added several interesting capabilites besides
actual modem-based communication.  It added:
- A BASIC processor, from which users could enter programs
- A facility to load and save these to non-volatile memory
- Sprite and Character graphic editors
- Support for a keyboard device

The Keyboard device is probably even more rare than the Tsuushin Booster itself, and I have
never heard any reports of seeing an actual device in person. However, we can infer a lot of
information from pictures which were published, from other computers of the era, and from
the way in which the Tsuushin Booster ROM reads (and interacts) with the (imaginary) device.

[More information on the Tsuushin Booster can be found here: https://www.chrismcovell.com/TsushinBooster/](https://www.chrismcovell.com/TsushinBooster/)

NOTE: Please do not ask me for the ROM; I will not share it. However, I happily share the results of my research.



### Differences from Modern Japanese Keyboards and Entry

First, I should mention that keyboards around the world have had many different layouts over the
decades. Even the "standard" North American English IBM PC keyboard layout was not alone prior to the
IBM PC becoming the most popular computer in North America. If you don't believe this, please take a
look at keyboard layouts for old typewriters or even computers like the Apple II or TRS-80 from the 1980's,
in particular the location of the "@" sign, quotation mark, apostrophe, and so on.

Japanese keyboards have additional keys along the bottom row (where the space bar is), in order to manage
multiple character sets.  Even these keys are not the same today as they were on, say a PC-88 keyboard in
the 1980's.

While many of the special Japanese keys still exist on a modern Japanese keyboard (such as HENKOU, KETTEI,
KANA), the "ZENKAKU" key is now labelled "HAN/ZEN KANJI" (for narrow or full-width characters), and all of
these keys have changed position and/or size/shape.

MISSING from today's keyboard, the following keys used to exist: GRPH, PC, and several keys on the numeric keypad
(',', '=', CLR and HELP). Also, STOP and COPY (to the left of the PC-88 function keys).

See next section for a picture of a PC-88 keyboard.

### PC-88 Similarities

Based on a rare photograph from a magazine scan during its development period, the Tsuushin Booster
keyboard was based directly on a PC-88 keyboard.  This shows from not only its shape and style, but
also the number of keys and their layout.  If the picture was clear enough, it would almost certainly
show the same lables on the keys.

Photo of Tsuushin Keyboard:

![Tsuushin Keyboard](images/TsuushinKeyboard.jpg)


Photo of PC-88 keyboard for comparison:

![PC-88 Keyboard](images/PC-88_keyboard.jpg)


## Software which uses it

Aside from the Tsuushin Booster, I am not aware of any software which could use the Tsuushin Keyboard as
an input device.  However, it might be interesting to use the information presented in this repository
for new software.


## Encoding Method

The similarities to the PC-88 keyboard don't end at the style and layout; the keypress information
is represented in an extremely similat way.

## Emulation

### Use of Macropad

Teso program for PCE Keyboard
