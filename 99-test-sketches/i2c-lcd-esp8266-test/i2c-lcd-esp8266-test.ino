#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // I2C address for LCD

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);  // Initialize 16x2 LCD
  lcd.setBacklight(255);  // Turn on the backlight

  lcd.setCursor(0, 0);
  lcd.print("Hello, ESP8266!");
}

void loop() {
  // Test basic display functionality
}
