# Tsuushin Keyboard

The Unreleased PC Engine Keyboard

## Test ROM

I have written a short program in HuC which will properly scan the kayeboard and diaplay
when keys are pressed or released.

## Screen shot

![Test ROM for Tsuushin Keyboard](images/PCE_Key_ROM.png)

## Program Code

The program source can be found in this repository in the Software/Test_ROM directory.
The RELEASE subdirectory conatins pre-compiled executables.

## Build Process

As of the time of writing, this requires the most receent version of HuC (development
version as at March 25, 2024 - not even published in a proper release yet) in order
to build correctly.

The reason for this is because programs written for HuC automatically call the
embedded joypad reading routine at every Vertical Sync interval (1/60 second).
In truth, we don't want that older jopyad routine to be used at all, replacing it
with our own.  The HuC library needed a small modification in order to make this
possible.


