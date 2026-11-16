#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address if necessary

// Initialize the SoftwareSerial for GSM communication
SoftwareSerial mySerial(9, 10);

// Define the analog pin for the current sensor
const int currentSensorPin = A0;

// Threshold for detecting a short circuit
const float shortCircuitThreshold = 2.0; // Adjust based on your sensor and requirement

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("System Initializing");

  // Initialize GSM Module
  mySerial.begin(9600); // Setting the baud rate of GSM Module
  Serial.begin(9600);   // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);

  gsmInit(); // Initialize GSM module

  // Display ready message
  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  // Read the current sensor value
  int sensorValue = analogRead(currentSensorPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // Debug: Print the sensor value to Serial Monitor
  Serial.print("Current Sensor Voltage: ");
  Serial.println(voltage);

  // Display current reading on LCD
  lcd.setCursor(0, 0);
  lcd.print("Current: ");
  lcd.print(voltage);

  // Check for short circuit
  if (voltage >= shortCircuitThreshold) {
    // Short circuit detected
    lcd.setCursor(0, 1);
    lcd.print("Short Circuit!");

    // Send SMS notification
    sendSMS("Short Circuit Detected!");

    // Wait for a while to avoid spamming messages
    delay(10000); // 10 seconds delay
  }

  // Small delay for the loop
  delay(1000);
}

void sendSMS(String msg) {
  mySerial.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(1000);
  mySerial.print("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Replace with mobile number
  delay(1000);
  mySerial.println(msg); // The SMS text you want to send
  delay(100);
  mySerial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void gsmInit() {
  bool at_flag = true;
  while (at_flag) {
    mySerial.println("AT");
    long timeout = millis() + 5000; // Timeout of 5 seconds
    while (mySerial.available() == 0 && millis() < timeout) {
      delay(10);
    }
    if (mySerial.find("OK")) {
      at_flag = false;
    } else {
      delay(1000);
    }
  }
  mySerial.println("ATE0");
  bool net_flag = true;
  while (net_flag) {
    mySerial.println("AT+CPIN?");
    long timeout = millis() + 5000; // Timeout of 5 seconds
    while (mySerial.available() == 0 && millis() < timeout) {
      delay(10);
    }
    if (mySerial.find("READY")) {
      net_flag = false;
    } else {
      delay(1000);
    }
  }
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CSMP=17,167,0,0");
  mySerial.flush();
}
