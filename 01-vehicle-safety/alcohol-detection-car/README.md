# Alcohol Detection Car

> A small car that locks its motors and sounds an alarm when alcohol is detected; LCD shows the sensor value.

**Board:** Arduino Uno / Nano (same pin names)

## Components required

- Arduino Uno / Nano
- MQ-3 alcohol sensor
- L298N motor driver + 2 DC motors (3-wheel chassis)
- 16x2 I2C LCD
- Buzzer, red LED, green LED
- Battery pack for motors

## Circuit / wiring

| Component | Component pin | Connect to (board pin) |
|---|---|---|
| MQ-3 module | AO | A0 |
| L298N | ENA | D11 |
| L298N | IN1 | D8 |
| L298N | IN2 | D9 |
| L298N | IN3 | D6 |
| L298N | IN4 | D7 |
| L298N | ENB | D5 |
| Buzzer | + | D4 |
| Red LED | anode via 220 ohm | D2 |
| Green LED | anode via 220 ohm | D3 |
| 16x2 I2C LCD | GND | GND |
| 16x2 I2C LCD | VCC | 5V |
| 16x2 I2C LCD | SDA | A4 |
| 16x2 I2C LCD | SCL | A5 |


_The pin numbers come straight from the `.ino` code. Double-check against your own hardware before powering on._

## Libraries needed

- `LiquidCrystal_I2C.h`
- `Wire.h`

## Code

- `alcohol-detection-car.ino`

## Working

1. The Arduino reads the MQ-3 sensor on A0 and shows `Alcohol: <value>` on the LCD.
2. **Above 500:** LCD shows *Alcohol Detected*, buzzer and red LED turn ON, and both motors are stopped (**engine locked**).
3. **At or below 500:** LCD shows *No Alcohol*, green LED turns ON and the motors run forward at full speed.
4. The check repeats every second.


## Notes

- The first LCD message is longer than 16 characters, so it is cut off on the display.
- Power the motors from a separate battery and join its GND with the Arduino GND.
