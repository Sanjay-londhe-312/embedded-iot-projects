#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // HC-05 TX, RX
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    BT.begin(9600);
    lcd.init();
    lcd.backlight();
}

void loop() {
    if (BT.available()) {
        String data = BT.readStringUntil('\n');
        int commaIndex = data.indexOf(',');
        
        if (commaIndex > 0) {
            String alcohol = data.substring(0, commaIndex);
            String water = data.substring(commaIndex + 1);
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alcohol: " + alcohol);
            lcd.setCursor(0, 1);
            lcd.print("Water: " + water);
        }
    }
}