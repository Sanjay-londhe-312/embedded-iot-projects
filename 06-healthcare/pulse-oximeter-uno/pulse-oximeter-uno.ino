#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30100_PulseOximeter.h"

// Constants
#define TEMP_PIN A1
#define REPORTING_PERIOD_MS 1000

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pulse Oximeter Setup
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Variables
bool sensorActive = false; // Track whether MAX30100 is providing data

// Callback for beat detection
void onBeatDetected() {
  Serial.println("Beat!!!");
}

void setup() {
  // Serial and LCD Initialization
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  // Initial LCD Message
  lcd.setCursor(0, 0);
  lcd.print("Health Monitor");
  delay(2000);
  lcd.clear();

  // Initialize Pulse Oximeter
  if (!pox.begin()) {
    Serial.println("Pulse Oximeter Init FAILED");
    while (1);
  } else {
    Serial.println("Pulse Oximeter Init SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  // Update Pulse Oximeter Data
  pox.update();
  
  // Check if the MAX30100 is sending valid data
  float heartRate = pox.getHeartRate();
  float spO2 = pox.getSpO2();
  sensorActive = (heartRate > 0 && spO2 > 0); // Valid data check
  
  // Periodic Updates
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    tsLastReport = millis();

    lcd.clear();
    if (sensorActive) {
      // Display Heart Rate and SpO2
      lcd.setCursor(0, 0);
      lcd.print("BPM: ");
      lcd.print(heartRate);
      lcd.setCursor(0, 1);
      lcd.print("SpO2: ");
      lcd.print(spO2);
      lcd.print("%");

      // Serial Monitor Output
      Serial.print("Heart Rate: ");
      Serial.print(heartRate);
      Serial.println(" BPM");
      Serial.print("SpO2: ");
      Serial.print(spO2);
      Serial.println(" %");
    } else {
      // Display Temperature
      int tempValue = analogRead(TEMP_PIN);
      float temperature = (tempValue * (3.3 / 1023.0)) * 100; // Convert to Celsius
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature, 1);
      lcd.print(" C");

      // Serial Monitor Output
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
    }
  }
}
