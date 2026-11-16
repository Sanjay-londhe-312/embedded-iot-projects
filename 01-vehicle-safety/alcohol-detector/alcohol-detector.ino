const int alcoholSensorPin = A1;
const int buzzerPin = 7;
const int redLedPin = 8;    
const int greenLedPin = 9;  

const int thresholdValue = 500;  
void setup() {
  Serial.begin(9600);

  pinMode(alcoholSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  int alcoholValue = analogRead(alcoholSensorPin);

  Serial.print("Alcohol: ");
  Serial.println(alcoholValue);

  if (alcoholValue > thresholdValue) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);

    // Output status on Serial Monitor
    Serial.println("Alcohol Detected");
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);

    // Output status on Serial Monitor
    Serial.println("No Alcohol Detected");
  }

  delay(1000);
}
