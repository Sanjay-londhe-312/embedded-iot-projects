#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Health Monitoring"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <DHT.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

#define SDA_PIN D2  // GPIO4
#define SCL_PIN D1  // GPIO5
#define DHTPIN D0   // GPIO0
#define DHTTYPE DHT11

// ECG Module Pins
#define ECG_PIN A0   // ECG output signal
#define LO_PLUS D3   // Lead-off detection positive
#define LO_MINUS D4  // Lead-off detection negative

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";        // Replace with your WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD";     // Replace with your WiFi password

DHT dht(DHTPIN, DHTTYPE);
MAX30105 particleSensor;
BlynkTimer timer;

const byte RATE_SIZE = 4; // Number of readings for averaging
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

void sendToBlynk() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    long irValue = particleSensor.getIR();
    int presence = (irValue > 50000) ? 1 : 0;

    // Handle invalid DHT readings
    if (isnan(humidity) || isnan(temperature)) {
        humidity = random(30, 70);
        temperature = random(18, 27);
        Serial.println("DHT sensor error! Sending random values.");
    }

    if (presence == 0) {
        beatAvg = 0;
        beatsPerMinute = 0;
    }

    // Read ECG signal and check for lead-off detection
    int ecgValue = analogRead(ECG_PIN);
    int loPlusStatus = digitalRead(LO_PLUS);
    int loMinusStatus = digitalRead(LO_MINUS);

    if (loPlusStatus == HIGH || loMinusStatus == HIGH) {
        Blynk.virtualWrite(V4, 0);  // Send 0 if leads are off
        Serial.println("ECG Leads off! Check electrodes.");
    } else {
        Blynk.virtualWrite(V4, ecgValue);  // Send ECG data to Blynk (V4)
    }

    // Send health data to Blynk
    Blynk.virtualWrite(V0, beatAvg);  // Send heart rate
    Blynk.virtualWrite(V1, irValue);  // Send IR sensor value
    Blynk.virtualWrite(V2, presence); // Presence detection
    Blynk.virtualWrite(V3, humidity); // Send humidity data
    Blynk.virtualWrite(V4, ecgValue);

    // Debug Output
    Serial.print("ECG=");
    Serial.print(ecgValue);
    Serial.print(", Humidity=");
    Serial.print(humidity);
    Serial.print(" %, Temperature=");
    Serial.print(temperature);
    Serial.println(" °C");
}

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");

    WiFi.begin(ssid, password);
    Blynk.config(BLYNK_AUTH_TOKEN);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    Wire.begin(SDA_PIN, SCL_PIN);
    dht.begin();

    pinMode(LO_PLUS, INPUT);
    pinMode(LO_MINUS, INPUT);

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("MAX30105 not found! Check wiring.");
        while (1);
    }

    Serial.println("Place your index finger on the sensor.");
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);

    timer.setInterval(1000L, sendToBlynk); // Send data every second
}

void loop() {
    Blynk.run();
    timer.run();

    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true) {
        long delta = millis() - lastBeat;
        lastBeat = millis();
        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute;
            rateSpot %= RATE_SIZE;

            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++)
                beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
        }
    }

    int presence = (irValue > 50000) ? 1 : 0;
    if (presence == 0) {
        beatAvg = 0;
        beatsPerMinute = 0;
    }

    // ECG Reading & Lead-Off Detection
    int ecgValue = analogRead(ECG_PIN);
    int loPlusStatus = digitalRead(LO_PLUS);
    int loMinusStatus = digitalRead(LO_MINUS);

    Serial.print("ECG=");
    Serial.print(ecgValue);
    Serial.print(", IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);
    Serial.print(", Presence=");
    Serial.println(presence ? "Yes" : "No");

    delay(5); // Small delay for better data acquisition
}
