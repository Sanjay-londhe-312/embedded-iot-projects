#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "irrrTemp"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Replace with your network credentials
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Virtual pins
#define VIRTUAL_PIN_TEMP V2
#define VIRTUAL_PIN_HUMIDITY V1

// DHT sensor settings
#define DHTPIN D3 // Pin where the DHT11 is connected
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup() {
  // Debug console
  Serial.begin(9600);

  // Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize the DHT sensor
  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensorData);
}

void sendSensorData() {
  // Reading temperature and humidity values from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Printing values to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C");
  Serial.print("  Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Sending temperature and humidity values to Blynk
  Blynk.virtualWrite(VIRTUAL_PIN_TEMP, temperature);
  Blynk.virtualWrite(VIRTUAL_PIN_HUMIDITY, humidity);
}

void loop() {
  Blynk.run();
  timer.run();
}
