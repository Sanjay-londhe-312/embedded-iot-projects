#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Ethernet"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"


#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// Pin Definitions
#define FIRE_SENSOR_PIN A0
#define HUMIDITY_SENSOR_PIN A1
#define MQ6_SENSOR_PIN A2
#define RELAY_FAN_PIN 7
#define RELAY_WATER_PUMP_PIN 8

// Variables to store sensor readings
int fireValue = 0;
int humidityValue = 0;
int mq6Value = 0;

// Blynk Virtual Pins
#define V_FLAME_SENSOR V0
#define V_HUMIDITY_SENSOR V1
#define V_SMOKE_SENSOR V3
#define V_FAN_CONTROL V4
#define V_PUMP_CONTROL V5

void setup() {
  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println("Initializing...");

  // Blynk initialization
  Blynk.begin(BLYNK_AUTH_TOKEN);

  // Pin configuration
  pinMode(FIRE_SENSOR_PIN, INPUT);
  pinMode(HUMIDITY_SENSOR_PIN, INPUT);
  pinMode(MQ6_SENSOR_PIN, INPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  pinMode(RELAY_WATER_PUMP_PIN, OUTPUT);

  // Initial relay state
  digitalWrite(RELAY_FAN_PIN, LOW);
  digitalWrite(RELAY_WATER_PUMP_PIN, LOW);

  Serial.println("System Initialized!");
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read sensors
  fireValue = analogRead(FIRE_SENSOR_PIN);
  humidityValue = analogRead(HUMIDITY_SENSOR_PIN);
  mq6Value = analogRead(MQ6_SENSOR_PIN);

  // Print sensor data to Serial Monitor
  Serial.print("Fire Sensor Value: ");
  Serial.println(fireValue);
  Serial.print("Humidity Sensor Value: ");
  Serial.println(humidityValue);
  Serial.print("MQ6 Sensor Value: ");
  Serial.println(mq6Value);

  // Send sensor data to Blynk
  Blynk.virtualWrite(V_FLAME_SENSOR, fireValue);
  Blynk.virtualWrite(V_HUMIDITY_SENSOR, humidityValue);
  Blynk.virtualWrite(V_SMOKE_SENSOR, mq6Value);

  // Small delay for stability
  delay(1000);
}

// Blynk Write Handlers for Fan and Pump Control
BLYNK_WRITE(V_FAN_CONTROL) {
  int fanState = param.asInt(); // Get state from Blynk app (0 or 1)
  digitalWrite(RELAY_FAN_PIN, fanState);
  Serial.print("Fan state: ");
  Serial.println(fanState ? "ON" : "OFF");
}

BLYNK_WRITE(V_PUMP_CONTROL) {
  int pumpState = param.asInt(); // Get state from Blynk app (0 or 1)
  digitalWrite(RELAY_WATER_PUMP_PIN, pumpState);
  Serial.print("Pump state: ");
  Serial.println(pumpState ? "ON" : "OFF");
}