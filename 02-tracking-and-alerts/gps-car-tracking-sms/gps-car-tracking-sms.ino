#include <TinyGPS++.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SoftwareSerial for GPS module
SoftwareSerial gpsSerial(10, 11); // RX, TX pins for GPS module

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
int temp = 0;

void setup() {
  Serial.begin(9600); // Serial for GSM and debugging
  gpsSerial.begin(GPSBaud); // GPS module communication
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Tracking");
  lcd.setCursor(0, 1);
  lcd.print("      System  ");
  delay(2000);

  gsm_init();

  lcd.clear();
  Serial.println("AT+CNMI=2,2,0,0,0");
  lcd.print("GPS Initializing");
  lcd.setCursor(0, 1);
  lcd.print("  No GPS Range  ");
  delay(2000);

  lcd.clear();
  lcd.print("GPS Range Found");
  lcd.setCursor(0, 1);
  lcd.print("GPS is Ready");
  delay(2000);

  lcd.clear();
  lcd.print("System Ready");
  temp = 0;
}

void loop() {
  serialEvent();

  while (temp) {
    while (gpsSerial.available() > 0) { // Use gpsSerial for GPS data
      char c = gpsSerial.read();
      gps.encode(c);
      if (gps.location.isUpdated()) {
        temp = 0;
        digitalWrite(13, HIGH);
        tracking();
      }
      if (!temp)
        break;
    }
  }
  digitalWrite(13, LOW);
}

void serialEvent() {
  while (Serial.available() > 0) {
    if (Serial.find("Track Vehicle")) {
      temp = 1;
      break;
    } else {
      temp = 0;
    }
  }
}

void gsm_init() {
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag = 1;
  while (at_flag) {
    Serial.println("AT");
    delay(1);
    while (Serial.available() > 0) {
      if (Serial.find("OK"))
        at_flag = 0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag = 1;
  while (echo_flag) {
    Serial.println("ATE0");
    while (Serial.available() > 0) {
      if (Serial.find("OK"))
        echo_flag = 0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag = 1;
  while (net_flag) {
    Serial.println("AT+CPIN?");
    while (Serial.available() > 0) {
      if (Serial.find("+CPIN: READY"))
        net_flag = 0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}

void init_sms() {
  Serial.println("AT+CMGF=1");
  delay(400);
  Serial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with your 10-digit phone number
  delay(400);
}

void send_data(String message) {
  Serial.print(message);
  delay(200);
}

void send_sms() {
  Serial.write(26); // Send Ctrl+Z to indicate end of SMS
}

void lcd_status() {
  lcd.clear();
  lcd.print("Message Sent");
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
}

void tracking() {
  init_sms();
  send_data("Vehicle Tracking Alert:");
  Serial.println(" ");
  send_data("Your Vehicle Current Location is:");
  Serial.println(" ");
  Serial.print("Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print("\n Longitude: ");
  Serial.println(gps.location.lng(), 6);

  Serial.print("https://www.google.com/maps/@");
  Serial.print(gps.location.lat(), 6);
  Serial.print(',');
  Serial.print(gps.location.lng(), 6);
  Serial.print(",14z");
  send_sms();
  delay(2000);
  lcd_status();
}
