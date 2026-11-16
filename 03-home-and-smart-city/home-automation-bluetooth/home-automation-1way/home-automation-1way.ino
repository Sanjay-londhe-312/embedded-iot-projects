#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX, TX

const int relayPin = 13; // Pin connected to the relay
char x;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure the relay is initially off
  
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available() > 0) {
    x = BTSerial.read();
    Serial.print("Received: ");
    Serial.println(x);
  }
  
  if (x == 'a') {
    digitalWrite(relayPin, HIGH); // Turn the bulb ON
    Serial.println("Bulb turned ON");
  }
  if (x == 'b') {
    digitalWrite(relayPin, LOW); // Turn the bulb OFF
    Serial.println("Bulb turned OFF");
  }
}
