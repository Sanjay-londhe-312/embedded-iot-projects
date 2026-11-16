#include <Servo.h>

Servo myservo;
const int trigPin = 6;
const int echoPin = 7;
long tmeduration;
int distance;

void setup() {
  myservo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure duration and calculate distance
  tmeduration = pulseIn(echoPin, HIGH);
  distance = (0.034 * tmeduration) / 2;

  // Check if a car is within 10 cm
  if (distance <= 10) {
    myservo.write(90);  // Open the gate
    Serial.println("Car detected. Gate opened.");
  } else {
    myservo.write(0);   // Close the gate
    Serial.println("No car detected. Gate closed.");
  }

  delay(1000);  // Adjust the delay based on your requirements
}
