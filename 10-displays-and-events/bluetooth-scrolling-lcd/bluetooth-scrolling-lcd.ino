#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SLAVE_ADDRESS 0x08 // the default address of the module is 0x08

LiquidCrystal_I2C lcd(0x27, 16, 2); // initialize the lcd with address 0x27 for a 16 chars and 2 line display
char lcdText[17]; // create a buffer to store the text for the lcd
int scrollCounter = 0; // counter to track the position of the text
boolean welcomeDisplayed = false; // flag to indicate whether the welcome message has been displayed

void setup() {
 Wire.begin(); // start the i2c communication as master
 Serial.begin(9600); // start the serial communication

 // wait for connection with bluetooth module
 while (!Serial);

 // initialize the lcd
 lcd.init();
 lcd.backlight();

 // display welcome message
 lcd.setCursor(3, 0);
 lcd.print("Welcome to");
 lcd.setCursor(1, 1);
 lcd.print("Notice Board");
 delay(2000); // wait for 2 seconds before continuing
}

void loop() {
 // display scrolling text
 while (Serial.available()) {
    if (!welcomeDisplayed) {
      // clear the lcd if the welcome message was displayed
      lcd.clear();
      welcomeDisplayed = true;
    }

    char receivedChar = Serial.read();

    // add the received character to the lcdText buffer
    for (int i = 0; i < 15; i++) {
      lcdText[i] = lcdText[i + 1];
    }
    lcdText[15] = receivedChar; // add the received character to the end of the buffer

    // update the display based on the contents of the lcdText buffer
    for (int i = 0; i < 16; i++) {
      lcd.setCursor((i + scrollCounter) % 16, 0);
      lcd.write(lcdText[i]);
    }

    // increment the scroll counter and wrap it around if necessary
    scrollCounter++;
    if (scrollCounter >= 16) {
      scrollCounter = 0;
    }

    // delay to control the scrolling speed
    delay(100);
 }
}