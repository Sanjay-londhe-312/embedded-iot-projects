#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Health Monitoring"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS  1000

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

LiquidCrystal_I2C lcd(0x27, 16, 2);
PulseOximeter pox;
uint32_t tsLastReport = 0;

byte smile[] = { B00000, B00000, B01010, B00000, B10001, B01110, B00000, B00000 };
byte mod[] = { B00000, B00000, B01010, B00000, B11111, B00000, B00000, B00000 };
byte sad[] = { B00000, B00000, B01010, B00000, B01110, B10001, B00000, B00000 };

void onBeatDetected() {
  Serial.println("Beat!!!");
}

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, smile);
  lcd.createChar(2, mod);
  lcd.createChar(3, sad);
  lcd.setCursor(0, 0);
  lcd.print("      Pulse");
  lcd.setCursor(0, 1);
  lcd.print("    Oximeter");
  delay(2000);

  // Connect to WiFi and Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize MAX30100 Sensor
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  Blynk.run();  // Run Blynk
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    // Send data to Blynk
    Blynk.virtualWrite(V0, bpm);
    Blynk.virtualWrite(V1, spo2);

    // Display data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM : ");
    lcd.print(bpm);
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(spo2);
    lcd.print("%");

    tsLastReport = millis();

    // Display icons based on SpO2 levels
    if (spo2 >= 96) {
      lcd.setCursor(15, 1);
      lcd.write(1);  // Smile face
    } else if (spo2 <= 95 && spo2 >= 91) {
      lcd.setCursor(15, 1);
      lcd.write(2);  // Neutral face
    } else if (spo2 <= 90) {
      lcd.setCursor(15, 1);
      lcd.write(3);  // Sad face
    }
  }
}
