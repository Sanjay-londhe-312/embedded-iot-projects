#include <SoftwareSerial.h>

// Initialize the SoftwareSerial for GSM communication
SoftwareSerial mySerial(7, 8);

void setup() {
  mySerial.begin(9600);    // Setting the baud rate of GSM Module
  Serial.begin(9600);      // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}

void loop() {
  // Check for incoming SMS
  if (mySerial.available()) {
    String message = mySerial.readString();
    message.trim();

    // Debug: Print the received message
    Serial.print("Received message: ");
    Serial.println(message);

    // Check for specific commands in received SMS
    if (message.equalsIgnoreCase("ON")) {
      // Code to turn motor on
      sendSMS("Motor turned ON");
    } else if (message.equalsIgnoreCase("OFF")) {
      // Code to turn motor off
      sendSMS("Motor turned OFF");
    }
  }
}

void sendSMS(String msg) {
  mySerial.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(1000);
  mySerial.print("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Replace with mobile number
  delay(1000);
  mySerial.println(msg); // The SMS text you want to send
  delay(100);
  mySerial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void gsmInit() {
  bool at_flag = true;
  while (at_flag) {
    mySerial.println("AT");
    long timeout = millis() + 5000; // Timeout of 5 seconds
    while (mySerial.available() == 0 && millis() < timeout) {
      delay(10);
    }
    if (mySerial.find("OK")) {
      at_flag = false;
    } else {
      delay(1000);
    }
  }
  mySerial.println("ATE0");
  bool net_flag = true;
  while (net_flag) {
    mySerial.println("AT+CPIN?");
    long timeout = millis() + 5000; // Timeout of 5 seconds
    while (mySerial.available() == 0 && millis() < timeout) {
      delay(10);
    }
    if (mySerial.find("READY")) {
      net_flag = false;
    } else {
      delay(1000);
    }
  }
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CSMP=17,167,0,0");
  mySerial.flush();
}
