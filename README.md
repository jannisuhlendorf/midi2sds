# midi2cv

# Velocity sensitive control for Simmons SDS 800 drum module

Note: This repository is a fork of https://github.com/elkayem/midi2cv with some modifications.

This repository contains the code and schematic for a DIY MIDI to drum-trigger converter. I use this for velocity sensitive control of my Simmons SDS 800 drum module via MIDI notes.

It's possible to trigger the SDS 800 using simple [trigger signals](https://en.wikipedia.org/wiki/CV/gate#Trigger), but this only give a constant drum velocity. With this project, it's possible to vary the drum velocity by modifying the voltage of a trigger signal. 

The MIDI to trigger converter has 4 outputs (MIDI notes C D E F). Each output is driven using a 12-bit DAC, the trigger length is 5 ms and the trigger voltage is proportional to the note velocity (between 0 and 4V).

## Parts
* Arduino Nano

SFH 618A-2
* Optocoupler (I used a Vishay SFH618A, but there are plenty of alternatives out there)
* 2x MCP4822 12-bit DACs
* LM324N Quad Op Amp 
* Diode (e.g., 1N917)
* 220, 500, 3x1K, 10K Ohm resistors
* 3x 0.1 uF ceramic capacitors
* 5 pin MIDI jack
* 7x 4mm banana plug jacks

The Arduino code uses the standard MIDI and SPI libraries, which can be found in the Arduino Library Manager. 

The schematic is illustrated at the bottom of this page (Eagle file included).  Input power (VIN) is 9-12V.

<img src="/images/schematic.JPG" alt="schematic" width="800">