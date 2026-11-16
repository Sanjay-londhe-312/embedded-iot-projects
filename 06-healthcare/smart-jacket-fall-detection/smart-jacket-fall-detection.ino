#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart jacket"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>

#define REPORTING_PERIOD_MS  1000
#define FALL_THRESHOLD 12.0  // Threshold for fall detection

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

Adafruit_MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint32_t tsLastReport = 0;

int heartRatePin = A0; // Define the pin for heart rate sensor

void setup() {
    Serial.begin(115200);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    
    // Initialize Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    
    // Initialize MPU6050
    if (!mpu.begin()) {
        Serial.println("MPU6050 initialization failed!");
        lcd.setCursor(0, 0);
        lcd.print("MPU6050 Error");
        while (1) delay(10);
    }
    Serial.println("MPU6050 initialized.");
    lcd.setCursor(0, 0);
    lcd.print("MPU6050 Ready");
    delay(2000);
}

void loop() {
    Blynk.run();
    
    // Read MPU6050 data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float accelerationMagnitude = sqrt(
        a.acceleration.x * a.acceleration.x +
        a.acceleration.y * a.acceleration.y +
        a.acceleration.z * a.acceleration.z
    );
    
    // Fall detection
    if (accelerationMagnitude > FALL_THRESHOLD) {
        Serial.println("Fall Detected!");
        Blynk.virtualWrite(V6, "Fall Detected!");
    } else {
        Blynk.virtualWrite(V6, "No Fall");
    }
    
    // Read heart rate sensor
    int heartRate = analogRead(heartRatePin);
    
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        // Print data to Serial Monitor
        Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.print(" C");
        Serial.print(" | X: "); Serial.print(a.acceleration.x);
        Serial.print(" | Y: "); Serial.print(a.acceleration.y);
        Serial.print(" | AccMag: "); Serial.print(accelerationMagnitude);
        Serial.print(" | BPM: "); Serial.println(heartRate);
        
        // Send data to Blynk
        Blynk.virtualWrite(V0, temp.temperature); // Temperature
        Blynk.virtualWrite(V1, a.acceleration.x); // X-axis acceleration
        Blynk.virtualWrite(V2, a.acceleration.y); // Y-axis acceleration
        Blynk.virtualWrite(V4, heartRate); // Heart rate
        
        // Display data on LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("X:");
        lcd.print(a.acceleration.x, 1);
        lcd.setCursor(8, 0);
        lcd.print("Y:");
        lcd.print(a.acceleration.y, 1);
        lcd.setCursor(0, 1);
        lcd.print("BPM:");
        lcd.print(heartRate);
        lcd.setCursor(8, 1);
        lcd.print("T:");
        lcd.print(temp.temperature, 1);
        lcd.print("C");
        
        tsLastReport = millis();
    }
    delay(500);
}
