#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
// Define the switch pin
const int switchPin = 2; // Change this to the pin where your switch is connected

// Create a SoftwareSerial object to communicate with the GSM module
SoftwareSerial gsmSerial(7, 8); // RX, TX

void setup() {
  // Initialize serial communication with the computer
  Serial.begin(9600);
  
  // Initialize serial communication with the GSM module
  gsmSerial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  pinMode(switchPin, INPUT);
}

void loop() {
  // Read the state of the switch
  int switchState = digitalRead(switchPin);
  lcd.setCursor(1,0);
  lcd.print("hello user");
  
  if (switchState == HIGH) {
    sendMessage(); 
    makeCall();
    delay(5000); 
    lcd.clear();
  }
}

void sendMessage() {
  gsmSerial.println("AT+CMGF=1"); 
  delay(1000);
  gsmSerial.print("AT+CMGS=\"+91XXXXXXXXXX\"\r"); 
  delay(1000);
  gsmSerial.print("please help me!! location : 18.1707589,73.3378236");
  delay(1000);
  gsmSerial.write(26); 
  delay(1000);
  lcd.setCursor(1,0);
  lcd.print("LAT:18.1707589");
  lcd.setCursor(1,1);
  lcd.print("LON:73.3378236");
  delay(1000);
}

void makeCall() {
  gsmSerial.println("ATD+91XXXXXXXXXX;"); // Replace with the phone number you want to call
  delay(1000);
 
}
