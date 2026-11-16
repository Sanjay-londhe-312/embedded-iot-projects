const int alcoholSensorPin = A1;
const int eyeBlinkSensorPin = A0;
const int buzzerPin = 4;
const int redLedPin = 2;    
const int greenLedPin = 3;  

const int thresholdValue = 500;  
const int eyeBlinkThreshold = 20;  // Adjust as needed

void setup() {
  Serial.begin(9600);
  pinMode(alcoholSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  // Alcohol sensor reading
  int alcoholValue = analogRead(alcoholSensorPin);
  Serial.print("Alcohol: ");
  Serial.println(alcoholValue);

  // Eye blink sensor reading
  int eyeBlinkSensorValue = analogRead(eyeBlinkSensorPin);
  float eyeBlinkRatio = map(eyeBlinkSensorValue, 0, 1023, 0, 100);
  Serial.print("Eye Blink Ratio (%): ");
  Serial.println(eyeBlinkRatio);

  // Check for drowsiness
  if (eyeBlinkRatio > eyeBlinkThreshold) {  // Adjusted threshold
    Serial.println("Driver might be drowsy!");
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000);
    delay(1000);
    digitalWrite(redLedPin, LOW);
    noTone(buzzerPin);
  }

  // Check for alcohol
  if (alcoholValue > thresholdValue) {
    Serial.println("Alcohol Detected");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    Serial.println("No Alcohol Detected");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  delay(1000);
}
