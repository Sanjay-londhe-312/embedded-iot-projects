// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library
#include <Keypad.h>

// Password Length
const int Password_Length = 4;
// Character to hold password input
String Data;
// Password
String Master = "1234";

// Pin connected to lock relay signal
int lockOutput = 11;

// Counter for character entries
byte data_count = 0;

// Character to hold key input
char customKey;

// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 4;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connections to Arduino
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Create LCD object: Use 0x27 If 0x3F Doesn't work
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();

  // Set lockOutput as an OUTPUT pin
  pinMode(lockOutput, OUTPUT);
}

void loop() {
  // Initialize LCD and print
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");

  // Look for keypress
  customKey = customKeypad.getKey();
  if (customKey) {
    // Enter keypress into array and increment counter
    Data += customKey;
    lcd.setCursor(data_count, 1);
    lcd.print('*'); // Display '*' for each entered character
    data_count++;
  }

  // See if we have reached the password length
  if (data_count == Password_Length) {
    lcd.clear();

    if (Data == Master) {
      // Correct Password
      lcd.print("Correct");

      // Toggle lock state
      digitalWrite(lockOutput, !digitalRead(lockOutput));

      delay(1000); // Delay to display "Correct" message
    } else {
      // Incorrect Password
      lcd.print("Incorrect");
      delay(1000);
    }

    // Clear data and LCD display
    lcd.clear();
    clearData();
  }
}

void clearData() {
  // Reset data_count
  data_count = 0;
  // Reset Data
  Data = "";
}
