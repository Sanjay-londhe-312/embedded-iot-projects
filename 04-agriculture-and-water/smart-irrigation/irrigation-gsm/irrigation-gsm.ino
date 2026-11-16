#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define SOIL_MOISTURE_PIN A0
#define TEMP_SENSOR_PIN A1
#define RELAY_PIN 8
#define GSM_TX 10
#define GSM_RX 9

SoftwareSerial gsm(GSM_RX, GSM_TX);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x27) if necessary

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Ensure pump is off initially

  lcd.init();
  lcd.backlight();

  Serial.println("System Initialized");
  lcd.setCursor(0, 0);
  lcd.print("System Init");
  sendSMS("Smart Irrigation System Initialized");

  delay(2000);
  lcd.clear();
}

void loop() {
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float temperature = analogRead(TEMP_SENSOR_PIN) * 0.48828125; // LM35 gives 10mV per degree Celsius
  
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(soilMoistureValue);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  
  if (soilMoistureValue < 300) { // Adjust threshold as necessary
    digitalWrite(RELAY_PIN, HIGH);  // Turn on pump
    sendSMS("Soil is dry, watering the plants");
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Turn off pump
  }

  delay(30000); // Check every 30 seconds
}

void sendSMS(String message) {
  gsm.print("AT+CMGF=1\r"); // Set SMS mode to text
  delay(1000);
  gsm.print("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Replace with your phone number
  delay(1000);
  gsm.print(message);
  delay(1000);
  gsm.write(26); // ASCII code of CTRL+Z
  delay(1000);
} 