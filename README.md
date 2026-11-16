# Embedded Systems & IoT Projects

Academic and hobby projects built with **Arduino Uno, NodeMCU (ESP8266) and ESP32-CAM**.
Each project has its own folder with the code, parts list, wiring and run steps so you can rebuild it yourself.

> All Wi-Fi names, passwords, Blynk tokens and phone numbers in this repo are **placeholders** (`YOUR_...`). Replace them with your own.

## Projects

### Vehicle Safety

- [Alcohol Detection Car](01-vehicle-safety/alcohol-detection-car) - A small car that locks its motors and sounds an alarm when alcohol is detected; LCD shows the sensor value.
- [Alcohol Detector](01-vehicle-safety/alcohol-detector) - MQ-3 sensor detects alcohol; buzzer and red LED warn above the threshold, green LED when safe.
- [Anti Sleep Alcohol Detection](01-vehicle-safety/anti-sleep-alcohol-detection) - Combines an eye-blink sensor and an alcohol sensor to warn a drowsy or drunk driver.
- [Eye Blink Sleep Detection](01-vehicle-safety/eye-blink-sleep-detection) - If the eyes stay closed for 3 seconds the relay cuts the vehicle and a buzzer sounds (basic and full vehicle versions).
- [Smart Bike](01-vehicle-safety/smart-bike) - Bike starts only after fingerprint match, no alcohol and a helmet check; relay controls ignition.
- [Toll Gate](01-vehicle-safety/toll-gate) - Ultrasonic sensor detects a car within 10 cm and a servo opens the barrier.

### Tracking And Alerts

- [Gps Car Tracking Sms](02-tracking-and-alerts/gps-car-tracking-sms) - Reads GPS coordinates and sends them by SMS through a GSM module.
- [Gps Gsm Button Alert](02-tracking-and-alerts/gps-gsm-button-alert) - Button press sends an SMS containing the current GPS location.
- [Gps Test](02-tracking-and-alerts/gps-test) - Simple GPS module test that prints coordinates on the Serial Monitor.
- [Gsm Sms Motor Control](02-tracking-and-alerts/gsm-sms-motor-control) - Turn a motor on/off by sending SMS commands to a GSM module.
- [Short Circuit Sms Alert](02-tracking-and-alerts/short-circuit-sms-alert) - Current sensor detects a short circuit, shows it on the LCD and sends an SMS.
- [Vehicle Tracking Accident Alert](02-tracking-and-alerts/vehicle-tracking-accident-alert) - GPS speed/location plus accelerometer; Arduino sends data to NodeMCU (Blynk map) and SMS alerts for over-speed or accident. Group project - add team credits.
- [Women Safety Sos](02-tracking-and-alerts/women-safety-sos) - Panic switch sends a help SMS with location and places a call.

### Home And Smart City

- [Alexa Google 4Relay Switch](03-home-and-smart-city/alexa-google-4relay-switch) - NodeMCU controls 4 relays by Alexa / Google Assistant using Sinric Pro, with manual switches.
- [Ethernet Fire Gas Automation](03-home-and-smart-city/ethernet-fire-gas-automation) - Flame, humidity and MQ-6 gas readings in Blynk over Ethernet; fan and pump switched from the app.
- [Home Automation Bluetooth](03-home-and-smart-city/home-automation-bluetooth) - Control AC, bulb, heater and fan through a 4-channel relay using Bluetooth/serial commands (4-way and 1-way versions).
- [Rain Sensor Servo](03-home-and-smart-city/rain-sensor-servo) - Servo moves when the rain sensor detects rain and returns when it stops.
- [Smart Dustbin](03-home-and-smart-city/smart-dustbin) - IR sensor opens the lid with a servo; ultrasonic sensor reports bin fill level to Blynk.
- [Smart Toilet Occupancy](03-home-and-smart-city/smart-toilet-occupancy) - Ultrasonic sensor detects presence, moves a servo and reports Occupied/Unoccupied to Blynk.

### Agriculture And Water

- [Load Cell Weight Dosing](04-agriculture-and-water/load-cell-weight-dosing) - HX711 load cell weighing with serial commands: zero, set <kg>, reset (plus calibration sketch).
- [Smart Irrigation](04-agriculture-and-water/smart-irrigation) - Soil moisture and temperature control a pump relay; GSM version also sends SMS.
- [Water Management](04-agriculture-and-water/water-management) - Multi-node Blynk system: three tank stations with pump control and two GPS-enabled water vehicles.

### Energy

- [Energy Monitor Thingspeak](05-energy/energy-monitor-thingspeak) - INA219 measures voltage, current and power, shows them on LCD and uploads to ThingSpeak.
- [Footstep Power Rfid Charging](05-energy/footstep-power-rfid-charging) - Footstep-generated power charges a power bank only when an authorised RFID card is scanned.

### Healthcare

- [Iot Health Monitor](06-healthcare/iot-health-monitor) - NodeMCU health monitor: heart rate, SpO2, ECG, temperature/humidity on LCD and Blynk.
- [Iot Syringe System](06-healthcare/iot-syringe-system) - Keypad and buttons set infusion parameters with Blynk connection. Motor logic is still a placeholder.
- [Max30105 Heart Rate](06-healthcare/max30105-heart-rate) - Heart-rate detection with MAX30105: serial, Blynk, humidity and ECG variants.
- [Pulse Oximeter Uno](06-healthcare/pulse-oximeter-uno) - MAX30100 heart rate and SpO2 shown on an LCD.
- [Smart Jacket Fall Detection](06-healthcare/smart-jacket-fall-detection) - MPU6050 fall detection with heart rate and temperature sent to Blynk.

### Safety

- [Coal Mine Helmet](07-safety/coal-mine-helmet) - Helmet monitors temperature, humidity and LPG gas; alarm and servo action on danger.
- [Lpg Gas Detector](07-safety/lpg-gas-detector) - MQ-6 LPG detection with LCD, buzzer and LEDs; variants with temperature/humidity.

### Security

- [Fingerprint Door Lock](08-security/fingerprint-door-lock) - Fingerprint sensor based door lock.
- [Keypad Door Lock](08-security/keypad-door-lock) - Password door lock with keypad, LCD, servo (or relay) and lock-out after wrong code.
- [Rfid Item Finder](08-security/rfid-item-finder) - RFID tags on items identified and displayed on an LCD with LED/buzzer.

### Robotics

- [Bluetooth Voice Car](09-robotics/bluetooth-voice-car) - 4-motor car driven by Bluetooth app or voice, with ultrasonic obstacle avoidance.
- [Esp32 Cam Ai Car](09-robotics/esp32-cam-ai-car) - ESP32-CAM streams video over WebSockets and takes drive/servo commands over UDP. Based on open-source code - add original credit and licence.
- [Esp32 Cam Wifi Car](09-robotics/esp32-cam-wifi-car) - ESP32-CAM web page with live video and on-screen car controls.
- [Joystick Car](09-robotics/joystick-car) - Joystick Arduino sends motor speeds over serial to a second Arduino driving the motors.

### Displays And Events

- [Bluetooth Master Slave](10-displays-and-events/bluetooth-master-slave) - Master Arduino sends alcohol and water sensor values over HC-05; slave shows them on LCD.
- [Bluetooth Scrolling Lcd](10-displays-and-events/bluetooth-scrolling-lcd) - Text typed in a Bluetooth app scrolls on a 16x2 LCD.
- [P10 Led Scrolling Display](10-displays-and-events/p10-led-scrolling-display) - Scrolling text on a P10 LED matrix using the DMD library.
- [Shiv Jayanti Demos](10-displays-and-events/shiv-jayanti-demos) - Event demos: servo fighting-soldier model and sequential LED effects.

### Test Sketches

- [Button Led Test](99-test-sketches/button-led-test) - Button press turns an LED/light on.
- [Dht11 Blynk Test](99-test-sketches/dht11-blynk-test) - DHT11 temperature and humidity sent to Blynk.
- [I2C Lcd Esp8266 Test](99-test-sketches/i2c-lcd-esp8266-test) - Hello-world test for an I2C LCD on ESP8266.
- [Keypad Test](99-test-sketches/keypad-test) - 4x4 keypad test with LCD output.
- [Relay Test 3](99-test-sketches/relay-test-3) - Toggles a relay every second.
- [Single Relay Test](99-test-sketches/single-relay-test) - Relay on/off from serial commands.

## How to run any project
1. Install the Arduino IDE 2.x.
2. NodeMCU: add the ESP8266 board package. ESP32-CAM: add the ESP32 board package (Boards Manager).
3. Open the project's `.ino` file and install the libraries listed in that project's README.
4. Replace the `YOUR_...` placeholders.
5. Choose board and port, click Upload, open Serial Monitor at 9600 baud.

Tip: if the LCD stays blank, try I2C address `0x3F` instead of `0x27`.

### Pin reference used in the project READMEs
- Every project README has a **Components** list, a **pin table** (taken from the code) and a **connection chart** image.
- I2C LCD / sensors: **Uno and Nano** SDA = A4, SCL = A5. **Mega** SDA = 20, SCL = 21. **NodeMCU** SDA = D2, SCL = D1.
- NodeMCU pin labels: D0 = GPIO16, D1 = GPIO5, D2 = GPIO4, D3 = GPIO0, D4 = GPIO2, D5 = GPIO14, D6 = GPIO12, D7 = GPIO13, D8 = GPIO15.
- Arduino Nano uses the same pin names as the Uno, so the Uno tables work for both.
- Charts are generated from the pin list in the code; they are connection charts, not Fritzing drawings.

## Repository layout
```
embedded-iot-projects/
├── README.md
├── LICENSE
├── .gitignore
├── 01-vehicle-safety/
├── 02-tracking-and-alerts/
├── 03-home-and-smart-city/
├── 04-agriculture-and-water/
├── 05-energy/
├── 06-healthcare/
├── 07-safety/
├── 08-security/
├── 09-robotics/
├── 10-displays-and-events/
├── 99-test-sketches/
    └── project-name/
        ├── project-name.ino
        └── README.md
```

## License
My original code is under the MIT License (see `LICENSE`).
Some projects use or adapt Arduino example code, open-source GitHub code and AI-assisted code.
That code stays under its original author's rights and license. See the notice at the bottom of
`LICENSE`, `THIRD_PARTY_NOTICES.md` and each project's README for credits.

## Author
Sanjay Londhe - [GitHub](https://github.com/your-username) - [LinkedIn](https://linkedin.com/in/your-profile)
