# Alcohol Detector

> MQ-3 sensor detects alcohol; buzzer and red LED warn above the threshold, green LED when safe.

**Board:** Arduino Uno / Nano (same pin names)

## Components required

- Arduino Uno / Nano
- MQ-3 alcohol sensor module
- Buzzer
- Red LED + Green LED
- 2 x 220 ohm resistors
- Breadboard, jumper wires, USB cable

## Circuit / wiring

| Component | Component pin | Connect to (board pin) |
|---|---|---|
| MQ-3 module | VCC | 5V |
| MQ-3 module | GND | GND |
| MQ-3 module | AO (analog out) | A1 |
| Buzzer | + | D7 |
| Buzzer | - | GND |
| Red LED | anode (+) via 220 ohm | D8 |
| Green LED | anode (+) via 220 ohm | D9 |
| Both LEDs | cathode (-) | GND |


_The pin numbers come straight from the `.ino` code. Double-check against your own hardware before powering on._

## Libraries needed

- None (built-in only)

## Code

- `alcohol-detector.ino`

## Working

1. The MQ-3 sensor changes its analog output with the alcohol vapour around it.
2. The Arduino reads it on **A1** every second and prints the value on the Serial Monitor.
3. If the value is **above 500** (the threshold), the **buzzer and red LED turn ON** and the green LED turns OFF.
4. Otherwise the green LED stays ON.
5. Change `thresholdValue` in the code to make it more or less sensitive.


## Notes

- Let the MQ-3 warm up for 2-3 minutes after power-on before trusting the readings.
- This is a learning project, not a certified breathalyser.
