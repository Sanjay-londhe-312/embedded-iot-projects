#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Water Management"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* pass = "YOUR_WIFI_PASSWORD";

// Ultrasonic sensor pins
const int trigPin = D5; // GPIO14
const int echoPin = D6; // GPIO12
const int maxLevelCm = 9;

// GPS module
SoftwareSerial gpsSerial(D7, D8); // RX, TX
TinyGPSPlus gps;

// Blynk timer
BlynkTimer timer;

// Store previous values
double lastLat = 0.0;
double lastLng = 0.0;
float lastLevelPercent = -1.0;

const float gpsChangeThreshold = 0.0001; // GPS precision (~11m)
const float levelChangeThreshold = 1.0;  // Minimum 1% change to trigger Blynk update

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(3000L, sendSensorData); // Check every 3 seconds
}

void loop() {
  Blynk.run();
  timer.run();

  // Keep reading GPS
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

void sendSensorData() {
  // --- Ultrasonic Reading ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * 0.0343 / 2;
  distanceCm = constrain(distanceCm, 0, maxLevelCm);

  float percentLevel = ((maxLevelCm - distanceCm) / maxLevelCm) * 100.0;

  // --- Ultrasonic Update to Blynk only if value changed significantly ---
  if (abs(percentLevel - lastLevelPercent) >= levelChangeThreshold) {
    Blynk.virtualWrite(V11, percentLevel);
    lastLevelPercent = percentLevel;

    Serial.print("Blynk Updated -> ");
  } else {
    Serial.print("No Blynk update -> ");
  }

  Serial.print("Water Level: ");
  Serial.print(distanceCm);
  Serial.print(" cm | ");
  Serial.print(percentLevel);
  Serial.println(" %");

  // --- GPS Reading ---
  if (gps.location.isValid()) {
    double currentLat = gps.location.lat();
    double currentLng = gps.location.lng();

    if (abs(currentLat - lastLat) > gpsChangeThreshold || abs(currentLng - lastLng) > gpsChangeThreshold) {
      Blynk.virtualWrite(V7, currentLat, 6);
      Blynk.virtualWrite(V8, currentLng, 6);

      lastLat = currentLat;
      lastLng = currentLng;

      Serial.print("Blynk GPS Updated -> Latitude: ");
      Serial.print(currentLat, 6);
      Serial.print(" | Longitude: ");
      Serial.println(currentLng, 6);
    } else {
      Serial.println("GPS unchanged. No Blynk update.");
    }
  } else {
    Serial.println("Waiting for GPS signal...");
  }
}
