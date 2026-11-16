#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows
Servo servoMotor; // Create a servo object

int GAS_VAL = 0;
int buzzerPin = 8;    // Buzzer connected to digital pin 8
int redLedPin = 7;     // Red LED connected to digital pin 7
int greenLedPin = 6;   // Green LED connected to digital pin 6
int servoPin = 9;      // Servo motor connected to digital pin 9

// Temperature sensor (DS18B20) related variables
#define ONE_WIRE_BUS 2 // Pin where the DS18B20 is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temperature;

// Humidity sensor (DHT22) related variables
#define DHT_PIN 3        // Pin where the DHT22 is connected
#define DHT_TYPE DHT22   // Type of the DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);
float humidity;

void setup()
{
  pinMode(A0, INPUT); // MQ-6 A0 Pin
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   GAS SENSOR  ");

  pinMode(redLedPin, OUTPUT);  // Red LED
  pinMode(greenLedPin, OUTPUT); // Green LED
  pinMode(buzzerPin, OUTPUT);   // Buzzer

  servoMotor.attach(servoPin); // Attaching the servo motor to the pin

  sensors.begin(); // Initialize the temperature sensor
  dht.begin();     // Initialize the humidity sensor
}

void loop()
{
  GAS_VAL = analogRead(A0);
  Serial.println(GAS_VAL);

  // Read temperature from the DS18B20 sensor
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Read humidity from the DHT22 sensor
  humidity = dht.readHumidity();

  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (GAS_VAL > 500)
  {
    lcd.setCursor(0, 1);
    lcd.print("  LPG Detected   ");
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000); // Turn on the buzzer at 1000 Hz

    // Move the servo motor to a specific angle (you can adjust this angle)
    servoMotor.write(90); // 90 degrees
    delay(1000); // Delay for servo movement
    servoMotor.write(0); // Return to the initial position
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
