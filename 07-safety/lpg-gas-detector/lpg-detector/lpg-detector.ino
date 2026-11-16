#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

int GAS_VAL = 0;
int buzzerPin = 8; // Buzzer connected to digital pin 8
int redLedPin = 7; // Red LED connected to digital pin 7
int greenLedPin = 6; // Green LED connected to digital pin 6

void setup()
{
  pinMode(A0, INPUT); // MQ-6 A0 Pin
  Serial.begin(9600);
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   GAS SENSOR  ");

  pinMode(redLedPin, OUTPUT); // Red LED
  pinMode(greenLedPin, OUTPUT); // Green LED
  pinMode(buzzerPin, OUTPUT); // Buzzer
}

void loop()
{
  GAS_VAL = analogRead(A0);
  Serial.println(GAS_VAL);

  if (GAS_VAL > 300)
  {
    lcd.setCursor(0, 1);
    lcd.print("  LPG Detected   ");
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000); // Turn on the buzzer at 1000 Hz
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("LPG Not Detected");
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    noTone(buzzerPin); // Turn off the buzzer
  }

  delay(10);
}
