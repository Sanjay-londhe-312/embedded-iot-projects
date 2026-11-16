#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "SmartToilet"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_NAME";   // Enter your WiFi name
char pass[] = "YOUR_WIFI_PASSWORD"; // Enter your WiFi password

const int trigPin = D1; // GPIO5
const int echoPin = D2; // GPIO4
const int servoPin = D3; // GPIO0

Servo myServo;

long duration;
int distance;

// Variable to store the occupancy status
String occupancyStatus = "Unoccupied";

// Function to send occupancy status to Blynk
void sendOccupancyStatus() {
  Blynk.virtualWrite(V0, occupancyStatus); // V1 is the virtual pin for occupancy status
}

void setup() {
  pinMode(D0, OUTPUT); // Assuming D0 is used for LED, adjust as necessary
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.write(0); // Initial position of the servo
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); // Initialize Blynk
}

void loop() {
  Blynk.run(); // Run Blynk

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 50) {
    Serial.println("Occupied"); // Debugging statement
    myServo.write(90); // Adjust the angle as per your servo's specifications
    occupancyStatus = "Occupied";
  } else {
    Serial.println("Unoccupied"); // Debugging statement
    myServo.write(0); // Close the servo
    occupancyStatus = "Unoccupied";
  }

  sendOccupancyStatus();

  delay(100); // Adjust delay based on your specific needs
}
