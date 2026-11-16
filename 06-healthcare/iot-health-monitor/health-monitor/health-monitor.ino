#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Health Monitoring"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

BlynkTimer timer;

// Constants
#define DHT_PIN D7       // Pin for DHT11 sensor
#define DHT_TYPE DHT11   // Specify DHT type
#define REPORTING_PERIOD_MS 1000

// LCD Setup
LiquidCrystal_PCF8574 lcd(0x27);

// Pulse Oximeter Setup
PulseOximeter pox;
uint32_t tsLastReport = 0;

// DHT11 Sensor Setup
DHT dht(DHT_PIN, DHT_TYPE);

// Pins for leads-off detection
#define LO_PLUS D3  
#define LO_MINUS D4 

// ECG buffer setup
int ecgBuffer[16] = {0}; // Buffer for ECG data
int bufferIndex = 0;      // Index for buffer
bool ecgUpdated = false;  // Flag to indicate new ECG data

// Callback for beat detection
void onBeatDetected() {
  Serial.println("Beat Detected!");
}

void setupWiFi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void setupBlynk() {
  // Connect to Blynk
  Serial.print("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Blynk");
}

void setupLCD() {
  lcd.begin(16, 2);
  lcd.setBacklight(255); // Set maximum brightness
  lcd.setCursor(0, 0);
  lcd.print("Health Monitor");
  delay(2000);
  lcd.clear();
}

void setupSensors() {
  // Initialize Pulse Oximeter
  if (!pox.begin()) {
    Serial.println("Pulse Oximeter Init FAILED");
    while (1); // Halt if sensor fails
  } else {
    Serial.println("Pulse Oximeter Init SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

  // Initialize leads-off detection pins
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);

  // Initialize DHT11 sensor
  dht.begin();
}

void displayHealthData(float heartRate, float spO2, float temperature, float humidity, int ecgValue) {
  static int displayPage = 0;

  lcd.clear();
  if (displayPage == 0) {
    // Display Heart Rate and SpO2
    lcd.setCursor(0, 0);
    lcd.print("BPM: ");
    lcd.print(heartRate);
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(spO2);
    lcd.print("%");
  } else if (displayPage == 1) {
    // Display Temperature and Humidity
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity, 1);
    lcd.print("%");
  } else if (displayPage == 2) {
    // Display ECG Value
    lcd.setCursor(0, 0);
    lcd.print("ECG: ");
    lcd.print(ecgValue);
    lcd.setCursor(0, 1);
    lcd.print("Leads: ");
    lcd.print((digitalRead(LO_PLUS) || digitalRead(LO_MINUS)) ? "OFF" : "ON");
  }

  // Cycle through pages
  displayPage = (displayPage + 1) % 3;
}

void updateECGData() {
  // Read ECG data from A0
  int ecgValue = analogRead(A0);
  int ecgHeight = map(ecgValue, 0, 1023, 0, 15);

  // Update the buffer with new ECG value
  ecgBuffer[bufferIndex] = ecgHeight;
  bufferIndex = (bufferIndex + 1) % 16;  // Circular buffer to keep the last 16 samples

  ecgUpdated = true;
  // Serial Monitor Output for raw ECG data
  Serial.print("ECG Value: ");
  Serial.println(ecgValue);

  // Send ECG data to Blynk
  Blynk.virtualWrite(V4, ecgValue);
}

void leadsOffDetection() {
  if ((digitalRead(LO_PLUS) == 1) || (digitalRead(LO_MINUS) == 1)) {
    Serial.println("Leads Off Detected!");
  }
}

void loop() {
  // Check if Wi-Fi and Blynk are connected before starting sensor operations
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    // Update Pulse Oximeter Data
    pox.update();
    float heartRate = pox.getHeartRate();
    float spO2 = pox.getSpO2();

    // Get Temperature and Humidity from DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Periodic Update
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      tsLastReport = millis();
      int ecgValue = analogRead(A0); // Get current ECG value
      displayHealthData(heartRate, spO2, temperature, humidity, ecgValue);

      ecgUpdated = false;  // Reset the flag after updating the ECG
    }

    // Update ECG Data
    updateECGData();
    leadsOffDetection();

    Blynk.run(); // Handle Blynk communication
    timer.run(); // Timer for other scheduled tasks
  } else {
    // If not connected, try to reconnect
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, reconnecting...");
      setupWiFi();
    }
    if (!Blynk.connected()) {
      Serial.println("Blynk disconnected, reconnecting...");
      setupBlynk();
    }
  }
}

void setup() {
  // Start Serial and initialize components
  Serial.begin(115200);
  
  // Setup Wi-Fi and Blynk after successful connection
  setupWiFi();
  setupBlynk();
  setupLCD();
  setupSensors();
}
