#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int alcoholSensorPin = A0;
const int motorSpeedPinA = 11; // Connect to ENA on the motor driver
const int motorSpeedPinB = 5;  // Connect to ENB on the motor driver
const int motorDirectionPin1 = 8;
const int motorDirectionPin2 = 9;
const int motorDirectionPin3 = 6;
const int motorDirectionPin4 = 7;
const int buzzerPin = 4;
const int redLedPin = 2;    // Red LED for alcohol detected (engine locked)
const int greenLedPin = 3;  // Green LED for car running

const int thresholdValue = 500;  // You can adjust the threshold value according to your requirements

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Alcohol Detection & Monitoring");

  pinMode(alcoholSensorPin, INPUT);
  pinMode(motorSpeedPinA, OUTPUT);
  pinMode(motorSpeedPinB, OUTPUT);
  pinMode(motorDirectionPin1, OUTPUT);
  pinMode(motorDirectionPin2, OUTPUT);
  pinMode(motorDirectionPin3, OUTPUT);
  pinMode(motorDirectionPin4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  int alcoholValue = analogRead(alcoholSensorPin);

  // Display alcohol sensor value on LCD
  lcd.clear();
  lcd.print("Alcohol: ");
  lcd.print(alcoholValue);

  // Display alcohol sensor value on Serial Monitor
  Serial.print("Alcohol: ");
  Serial.println(alcoholValue);

  if (alcoholValue > thresholdValue) {
    lcd.setCursor(0, 1);
    lcd.print("Alcohol Detected");

    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);

    // Lock the engine by stopping the motors
    stopMotors();

    // Output status on Serial Monitor
    Serial.println("Engine Locked - Alcohol Detected");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("No Alcohol");

    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);

    // Allow the engine to run
    runMotors();

    // Output status on Serial Monitor
    Serial.println("Car Running - No Alcohol Detected");
  }

  delay(1000);
}

void stopMotors() {
  digitalWrite(motorSpeedPinA, LOW);
  digitalWrite(motorSpeedPinB, LOW);
}

void runMotors() {
  digitalWrite(motorDirectionPin1, HIGH);
  digitalWrite(motorDirectionPin2, LOW);
  digitalWrite(motorDirectionPin3, HIGH);
  digitalWrite(motorDirectionPin4, LOW);

  analogWrite(motorSpeedPinA, 255);
  analogWrite(motorSpeedPinB, 255);
}
