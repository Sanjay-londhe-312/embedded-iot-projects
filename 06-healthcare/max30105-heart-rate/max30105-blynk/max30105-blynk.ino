1#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Health Monitoring"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
#define SDA_PIN D2  // GPIO4
#define SCL_PIN D1  // GPIO5

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

const char* ssid = "YOUR_WIFI_NAME";        // Replace with your WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your WiFi password


MAX30105 particleSensor;

const byte RATE_SIZE = 4; // Number of readings for averaging
byte rates[RATE_SIZE];    
byte rateSpot = 0;
long lastBeat = 0;        

float beatsPerMinute;
int beatAvg;

BlynkTimer timer;

void sendToBlynk() {
  Blynk.virtualWrite(V0, beatAvg);  // Send heart rate
  Blynk.virtualWrite(V1, 98);       // Placeholder SpO2 (adjust if calculated)
  Blynk.virtualWrite(V2, (particleSensor.getIR() > 50000) ? 1 : 0); // Presence detection
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  WiFi.begin(ssid, password);
  Blynk.config(BLYNK_AUTH_TOKEN);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C with ESP8266 pins

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
  {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(); 
  particleSensor.setPulseAmplitudeRed(0x0A); // Low Red LED brightness
  particleSensor.setPulseAmplitudeGreen(0);  // Green LED off

  timer.setInterval(1000L, sendToBlynk);  // Send data to Blynk every second
}

void loop()
{
  Blynk.run();
  timer.run();

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
  Serial.print(", Presence=");
  Serial.print(irValue > 50000 ? "Yes" : "No");

  Serial.println();
}
