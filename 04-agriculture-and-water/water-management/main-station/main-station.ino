#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Water Management"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Blynk Credentials
char auth[] = BLYNK_AUTH_TOKEN;

// Pin Definitions
#define TRIG_PIN     14  // D5
#define ECHO_PIN     12  // D6
#define TOGGLE_PIN    4  // D2
#define RELAY_PIN    13  // D7

// Tank Parameters
#define MAX_DISTANCE    17  // Max ultrasonic range in cm
#define FULL_LEVEL_CM   12  // Tank is full when 12cm from sensor
#define MIN_LEVEL_CM     1  // Tank is empty

// Objects
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// State Variables
bool blynkToggle = false;
bool dispensing = false;
float startLevel = 0;
float dispensedPercent = 0;
int lastSentLevel = -1;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Wire.begin(2, 0);  // SDA=D2, SCL=D1

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF initially
  pinMode(TOGGLE_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);

  lcd.clear();
}

// Blynk virtual pin V3 = dispense toggle
BLYNK_WRITE(V3) {
  int value = param.asInt();
  blynkToggle = (value == 1);
}

void loop() {
  Blynk.run();

  float distance = readDistance();
  if (distance <= 0 || distance >= MAX_DISTANCE) {
    handleSensorError();
    return;
  }

  float level = calculateWaterLevel(distance);
  bool toggleOn = digitalRead(TOGGLE_PIN) == LOW;

  displayWaterLevel(level);
  controlPump(level, distance, toggleOn || blynkToggle);
  displayPumpStatus();
  checkAndTriggerAlarm(level);

  int currentLevel = (int)level;
  if (currentLevel != lastSentLevel) {
    Blynk.virtualWrite(V2, currentLevel);
    Serial.println("V2 updated");
    lastSentLevel = currentLevel;
  }

  delay(200);
}

// ----------------- FUNCTIONS -----------------

float readDistance() {
  delay(50);
  return sonar.ping_cm();
}

void handleSensorError() {
  lcd.setCursor(0, 0);
  lcd.print("Sensor Error     ");
  lcd.setCursor(0, 1);
  lcd.print("Pump: OFF        ");
  Serial.println("Sensor Error");
  digitalWrite(RELAY_PIN, HIGH);
  dispensing = false;
}

float calculateWaterLevel(float distance) {
  float level = (FULL_LEVEL_CM - distance) / (FULL_LEVEL_CM - MIN_LEVEL_CM) * 100.0;
  return constrain(level, 0, 100);
}

void displayWaterLevel(float level) {
  lcd.setCursor(0, 0);
  lcd.print("Water: ");
  lcd.print((int)level);
  lcd.print("%     ");
  Serial.print("Level: ");
  Serial.print(level);
  Serial.println("%");
}

void controlPump(float level, float distance, bool controlEnabled) {
  if (controlEnabled && !dispensing) {
    startLevel = level;
    if (startLevel >= 60) {
      dispensing = true;
      digitalWrite(RELAY_PIN, LOW);  // Pump ON
      Serial.println("Dispensing started");
    } else {
      Serial.println("Tank not enough to dispense");
    }
  }

  if (dispensing) {
    float targetLevel = startLevel - 60;
    if (level <= targetLevel || level <= 0) {
      dispensing = false;
      digitalWrite(RELAY_PIN, HIGH);  // Pump OFF
      dispensedPercent = startLevel - level;
      Serial.print("Dispensed: ");
      Serial.print(dispensedPercent);
      Serial.println("%");
    }
  }
}

void displayPumpStatus() {
  lcd.setCursor(0, 1);
  if (dispensing) {
    lcd.print("Dispensing...   ");
  } else {
    lcd.print("Last Out: ");
    lcd.print((int)dispensedPercent);
    lcd.print("%   ");
  }
}

void checkAndTriggerAlarm(float level) {
  if (level < 10) {
    Blynk.virtualWrite(V6, 255);  // Alarm ON
  } else {
    Blynk.virtualWrite(V6, 0);    // Alarm OFF
  }
}
