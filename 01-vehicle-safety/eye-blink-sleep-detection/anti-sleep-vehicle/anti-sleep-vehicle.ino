#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int eyeBlinkSensorPin = A1; // Eye blink sensor output connected to analog pin A1
const int relayPin = 2;           // Relay module control pin
const int buzzerPin = 4;          // Buzzer pin
const int redLEDPin = 3;          // Red LED pin
const int yellowLEDPin = 5;       // Yellow LED pin

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

unsigned long blinkStartTime = 0;   // Variable to store the start time of eye closure
const int drowsyThreshold = 20;     // Eye blink ratio threshold for drowsiness
const unsigned long drowsyTime = 3000; // Time in milliseconds to trigger drowsiness alert (3 seconds)

void setup() {
  Serial.begin(9600);             // Initialize serial communication for debugging
  pinMode(relayPin, OUTPUT);      // Set relay pin as output
  pinMode(buzzerPin, OUTPUT);     // Set buzzer pin as output
  pinMode(redLEDPin, OUTPUT);     // Set red LED pin as output
  pinMode(yellowLEDPin, OUTPUT);  // Set yellow LED pin as output
  
  lcd.begin(16, 2);               // Initialize the LCD with 16 columns and 2 rows
  lcd.backlight();                // Turn on the backlight

  // Initial states
  digitalWrite(relayPin, LOW);   // Assuming relay is active-low
  digitalWrite(buzzerPin, LOW);   // Buzzer off
  digitalWrite(redLEDPin, LOW);   // Red LED off
  digitalWrite(yellowLEDPin, HIGH); // Yellow LED on
}

void loop() {
  int eyeBlinkSensorValue = analogRead(eyeBlinkSensorPin);

  // Convert sensor reading to meaningful value
  float eyeBlinkRatio = map(eyeBlinkSensorValue, 0, 1023, 0, 100);

  // Print sensor value to serial monitor
  Serial.print("Eye Blink Ratio (%): ");
  Serial.println(eyeBlinkRatio);

  lcd.setCursor(0, 0);  // Set cursor to first column, first row
  lcd.print("Eye Blink Ratio:");
  lcd.setCursor(0, 1);  // Set cursor to first column, second row
  lcd.print(eyeBlinkRatio);
  lcd.print("%   ");    // Clear any extra characters

  // Check for drowsiness
  if (eyeBlinkRatio < drowsyThreshold) { // Adjust threshold as needed
    if (blinkStartTime == 0) {
      // Start the timer when the threshold is first crossed
      blinkStartTime = millis();
    } else if (millis() - blinkStartTime >= drowsyTime) {
      // If the threshold is crossed for more than 3 seconds, trigger the alert
      Serial.println("Driver might be drowsy!");
      lcd.setCursor(0, 1);
      lcd.print("Driver Drowsy! ");

      digitalWrite(relayPin, LOW);   // Stop the car (assuming active-low relay)
      digitalWrite(buzzerPin, HIGH); // Turn on buzzer
      digitalWrite(redLEDPin, HIGH); // Turn on red LED
      digitalWrite(yellowLEDPin, LOW); // Turn off yellow LED
    }
  } else {
    // Reset the timer if the eye blink ratio is above the threshold
    blinkStartTime = 0;
    
    Serial.println("Driver is alert.");
    lcd.setCursor(0, 1);
    lcd.print("Driver Alert    ");

    digitalWrite(relayPin, HIGH);   // Start the car (assuming active-low relay)
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    digitalWrite(redLEDPin, LOW);   // Turn off red LED
    digitalWrite(yellowLEDPin, HIGH); // Turn on yellow LED
  }

  delay(100); // Delay between sensor readings
}
