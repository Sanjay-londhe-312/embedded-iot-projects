#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial Gsm(9, 10);           // RX, TX pins for GSM module
char phone_no[] = "+91*********";  // Replace with your phone number
TinyGPS gps;
int buttonState;
unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 500;  // Debounce time in milliseconds
bool isSMSsent = false;

void setup() {
  Serial.begin(9600);
  Gsm.begin(9600);

  // Configure GSM module
  Gsm.print("AT+CMGF=1\r");
  delay(100);
  Gsm.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  pinMode(4, INPUT_PULLUP);  // Configure button pin with internal pull-up resistor
}

void loop() {
  bool newData = false;

  // Read GPS data
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (Serial.available()) {
      char c = Serial.read();
      Serial.print(c);  // Output GPS data to the Serial Monitor
      if (gps.encode(c))
        newData = true;  // Check if there is new GPS data
    }
  }

  // Check button press
  buttonState = digitalRead(4);

  if (buttonState == LOW && millis() - lastButtonPressTime > debounceDelay) {
    lastButtonPressTime = millis();  // Update the last button press time

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

    if (flat == TinyGPS::GPS_INVALID_F_ANGLE || flon == TinyGPS::GPS_INVALID_F_ANGLE) {
      Serial.println("Invalid GPS data. SMS not sent.");
    } else {
      // Send SMS with location
      Gsm.print("AT+CMGF=1\r");
      delay(400);
      Gsm.print("AT+CMGS=\"+91XXXXXXXXXX\"");
      Gsm.println("Alert: I Need Help...");
      Gsm.print("http://maps.google.com/maps?q=loc:");
      Gsm.print(flat, 6);
      Gsm.print(",");
      Gsm.print(flon, 6);
      delay(200);
      Gsm.println((char)26);  // End of message
      delay(200);

      Serial.println("SMS Sent");
      isSMSsent = true;
    }
  }
}
