// Define joystick pins
const int joyXPin = A0; // Analog pin for X axis
const int joyYPin = A1; // Analog pin for Y axis
const int joySwPin = 2; // Digital pin for joystick switch (optional)

void setup() {
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // Read joystick values
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  int joySwValue = digitalRead(joySwPin);

  // Map joystick values to motor speeds (adjust as needed)
  int motorSpeedA = map(joyYValue, 0, 1023, -255, 255);
  int motorSpeedB = map(joyYValue, 0, 1023, -255, 255);

  // Map joystick X axis to control direction (adjust as needed)
  if (joyXValue < 500) {
    // Turn left
    motorSpeedA = -motorSpeedA;
  } else if (joyXValue > 550) {
    // Turn right
    motorSpeedB = -motorSpeedB;
  }

  // Send motor speeds to second Arduino via Serial
  Serial.print(motorSpeedA);
  Serial.print(",");
  Serial.println(motorSpeedB);

  delay(100); // Adjust delay as needed for responsiveness
}
