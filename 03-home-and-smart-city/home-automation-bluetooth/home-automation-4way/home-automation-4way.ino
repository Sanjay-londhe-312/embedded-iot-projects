#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX, TX

long int ac = 4;
long int bulb = 5;
long int heater = 6;
long int fan = 7;
char x;

void setup() {
  pinMode(ac, OUTPUT);
  pinMode(bulb, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(ac, LOW);
  digitalWrite(bulb, LOW);
  digitalWrite(heater, LOW);
  digitalWrite(fan, LOW);
  
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
    digitalWrite(ac, HIGH);
    Serial.println("AC turned ON");
  }
  if (x == 'b') {
    digitalWrite(ac, LOW);
    Serial.println("AC turned OFF");
  }
  if (x == 'c') {
    digitalWrite(bulb, HIGH);
    Serial.println("Bulb turned ON");
  }
  if (x == 'd') {
    digitalWrite(bulb, LOW);
    Serial.println("Bulb turned OFF");
  }
  if (x == 'e') {
    digitalWrite(heater, HIGH);
    Serial.println("Heater turned ON");
  }
  if (x == 'f') {
    digitalWrite(heater, LOW);
    Serial.println("Heater turned OFF");
  }
  if (x == 'g') {
    digitalWrite(fan, HIGH);
    Serial.println("Fan turned ON");
  }
  if (x == 'h') {
    digitalWrite(fan, LOW);
    Serial.println("Fan turned OFF");
  }
}
