const int eyeBlinkSensorPin = A1; // Eye blink sensor output connected to analog pin A0

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  int eyeBlinkSensorValue = analogRead(eyeBlinkSensorPin);

  // Convert sensor reading to meaningful value
  float eyeBlinkRatio = map(eyeBlinkSensorValue, 0, 1023, 0, 100);

  // Print sensor value to serial monitor
  Serial.print("Eye Blink Ratio (%): ");
  Serial.println(eyeBlinkRatio);
  
  // Check for drowsiness
  if (eyeBlinkRatio < 20) { // Adjust threshold as needed
    Serial.println("Driver might be drowsy!");
    // Add alert mechanism here (e.g., LED, buzzer)
  }
  
  delay(1000); // Delay between sensor readings
}
