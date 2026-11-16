#include <Servo.h>

const int rainSensorPin = A0;
const int servoPin = 9;

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(A0,INPUT);
}

void loop() {
  int rainValue = analogRead(A0);

  int Value = map(rainValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
 Serial.println(Value);

  if (Value > 170) {
    // It's raining, continuously move the servo from 0 to 90 degrees
    for (int angle = 0; angle <= 90; angle++) {
      myServo.write(angle);
      delay(10); // Adjust the delay as needed

      // Check if it has stopped raining
      if (analogRead(rainSensorPin) <= 170) {
        break; // Exit the loop if it's no longer raining
      }
    }
  } else {
    // It's not raining, move the servo to 0 degrees
    myServo.write(0);
  }

  delay(500);
}
