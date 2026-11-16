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
#define TRIG_PIN     14  // D5 (GPIO14)
#define ECHO_PIN     12  // D6 (GPIO12)
#define TOGGLE_PIN   4   // D2 (GPIO4)
#define RELAY_PIN    13  // D7 (GPIO13)

// Tank Parameters
#define MAX_DISTANCE    17  // Max ultrasonic range in cm
#define FULL_LEVEL_CM   12  // Tank is full when 12cm from sensor
#define MIN_LEVEL_CM    1   // Tank is empty

// Objects
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// State Variables
bool filling = false;
bool blynkToggle = false;
unsigned long lastUpdate = 0;
int lastSentLevel = -1;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Wire.begin(2, 0);  // SDA=D2 (GPIO4), SCL=D1 (GPIO5)

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF initially (active LOW)
  pinMode(TOGGLE_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);

  lcd.clear();
}

// Blynk virtual pin V5 = pump toggle
BLYNK_WRITE(V1) {
  int value = param.asInt();
  blynkToggle = (value == 1);
}

void checkAndTriggerAlarm(float level) {
  if (level < 10) {
    Blynk.virtualWrite(V6, 255);  // Turn alarm ON
  } else {
    Blynk.virtualWrite(V6, 0);    // Turn alarm OFF
  }
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

  int currentLevel = (int)level;
  if (currentLevel != lastSentLevel) {
    Blynk.virtualWrite(V0, currentLevel);
    Serial.println("V0 updated");
    lastSentLevel = currentLevel;
  }

  delay(200);
}

// --------------- FUNCTIONS BELOW ---------------

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
  digitalWrite(RELAY_PIN, HIGH);  // Turn OFF pump
  filling = false;
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
  if (controlEnabled && !filling && level <= 30) {
    filling = true;
    digitalWrite(RELAY_PIN, LOW);  // Turn ON pump
    Serial.println("Pump ON");
  }

  if (filling && (level >= 70 || distance <= 5)) {
    filling = false;
    digitalWrite(RELAY_PIN, HIGH);  // Turn OFF pump
    Serial.println("Pump OFF");
    checkAndTriggerAlarm(level);
  }
}

void displayPumpStatus() {
  lcd.setCursor(0, 1);
  lcd.print("Pump: ");
  lcd.print(filling ? "ON " : "OFF");
  lcd.print("         ");
}
