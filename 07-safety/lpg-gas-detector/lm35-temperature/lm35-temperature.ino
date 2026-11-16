#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD, set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int tempPin = A1;  // LM35 sensor pin (note: same as pulsePin, read sequentially)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the LCD with the correct number of columns and rows
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Print initial message
  lcd.setCursor(3, 0);
  lcd.print(" Temp");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature sensor value
  int tempValue = analogRead(tempPin);
  
  // Convert the analog reading to temperature in Celsius
  float temperature = (tempValue * (3.3 / 1023.0)) * 100;

  // Display the temperature on the serial monitor and LCD
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  lcd.setCursor(2, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  // Wait for 1 second before the next reading
  delay(1000);
}