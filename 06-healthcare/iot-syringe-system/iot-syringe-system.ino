#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <BlynkSimpleEsp8266.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Blynk setup
#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "IoT Syringe System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

char ssid[] = "YOUR_WIFI_NAME";          // Replace with your WiFi SSID
char pass[] = "YOUR_WIFI_PASSWORD";      // Replace with your WiFi password

// Keypad setup
const byte ROWS = 4;  // Four rows
const byte COLS = 4;  // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {D6, D7, D8, D9};  // Connect to keypad row pins
byte colPins[COLS] = {D10, D11, D12, D13};  // Connect to keypad column pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Button pins
#define START_PIN D9
#define RESET_PIN D11
#define STOP_PIN D10

void setup() {
  Blynk.begin(auth, ssid, pass);
  lcd.begin();
  lcd.backlight();

  // Button setup
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(STOP_PIN, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
}

void loop() {
  Blynk.run();

  // Handle button presses
  if (digitalRead(START_PIN) == LOW) {
    lcd.setCursor(0, 1);
    lcd.print("Infusion Started");
    // Add logic for motor start
  }

  if (digitalRead(RESET_PIN) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Reset");
    // Reset parameters and stop infusion
  }

  if (digitalRead(STOP_PIN) == LOW) {
    lcd.setCursor(0, 1);
    lcd.print("Infusion Stopped");
    // Add logic for motor stop
  }

  // Handle keypad input
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Key Pressed:");
    lcd.setCursor(0, 1);
    lcd.print(key);
    // Add logic for infusion rate/time setting
  }
}
