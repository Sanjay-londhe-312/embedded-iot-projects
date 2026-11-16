#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

// Define pins for modules and components
#define RELAY_PIN 11
#define ALCOHOL_SENSOR_PIN A1
#define RED_LED_PIN 10
#define GREEN_LED_PIN 9
#define YELLOW_LED_PIN 8
#define BUZZER_PIN 13
#define TRIG_PIN 4
#define ECHO_PIN 5
#define THRESHOLD_VALUE 300

// Initialize objects and variables
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial fingerPrint(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
uint8_t id;

bool systemAuthorized = false;
bool systemRunning = false;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    delay(1000);

    // Initialize pins for fingerprint module
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize pins for alcohol sensor and indicators
    pinMode(ALCOHOL_SENSOR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Turn off LEDs initially
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);

    // Initialize LCD display
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("  Welcome To");
    lcd.setCursor(0, 1);
    lcd.print("SD Bike");
    delay(2000);
    lcd.clear();
    lcd.print("Initializing...");
    delay(1000);

    // Initialize fingerprint module
    finger.begin(57600);
    lcd.clear();
    lcd.print("Finding Module");
    lcd.setCursor(0, 1);
    delay(1000);

    // Verify fingerprint module
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
        lcd.clear();
        lcd.print("Found Module ");
        delay(1000);
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        lcd.clear();
        lcd.print("Module not Found");
        lcd.setCursor(0, 1);
        lcd.print("Check Connections");
        while (1);
    }
}

void loop() {
    float adcValue = 0;

    // Read analog sensor for alcohol detection
    for (int i = 0; i < 10; i++) {
        adcValue += analogRead(ALCOHOL_SENSOR_PIN);
        delay(10);
    }
    float voltage = (adcValue / 10) * (5.0 / 1024.0);
    float mgL = 0.67 * voltage;
    Serial.print("BAC:");
    Serial.print(mgL);
    Serial.print(" mg/L");

    // Alcohol detection logic
    int alcoholValue = analogRead(ALCOHOL_SENSOR_PIN);
    Serial.print(" Alcohol: ");
    Serial.println(alcoholValue);

    if (alcoholValue > THRESHOLD_VALUE) {
        // Alcohol detected
        Serial.println("    Alcohol Detected");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alcohol Detected");
        lcd.setCursor(0, 1);
        lcd.print("unable to drive");
        digitalWrite(RED_LED_PIN, HIGH);  // Turn on red LED
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        delay(1000);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        delay(1000);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
        return;
    } else {
        // No alcohol detected
        Serial.println("    No Alcohol Detected");
        digitalWrite(RED_LED_PIN, LOW);  // Turn off red LED
        digitalWrite(BUZZER_PIN, LOW);
    }

    // If system is authorized and not running, start the system
    if (systemAuthorized && !systemRunning) {
        // Check alcohol level before starting
        if (mgL > 1.3) {
            // Driver is drunk, system cannot start
            Serial.println("Driver is drunk. Unable to drive.");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Driver is Drunk");
            lcd.setCursor(0, 1);
            lcd.print("alcoholValue");                  
            digitalWrite(RED_LED_PIN, HIGH);  // Turn on red LED
            digitalWrite(RELAY_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, HIGH);
            delay(2000);
            digitalWrite(BUZZER_PIN, LOW);
            return;
        }

        // Check if helmet is worn before starting
        if (!isHelmetWorn()) {
            // Helmet is not worn, system cannot start
            Serial.println("Helmet not worn. System cannot start.");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Wear Helmet");
            lcd.setCursor(0, 1);
            lcd.print("System Stopped");
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Turn on yellow LED
            digitalWrite(RELAY_PIN, LOW);
            digitalWrite(BUZZER_PIN, HIGH);
            delay(2000);
            digitalWrite(BUZZER_PIN, LOW);
            return;
        }

        // Start the system
        digitalWrite(RELAY_PIN, HIGH);
        lcd.clear();
        lcd.print("System Started");
        lcd.setCursor(0, 1);
        lcd.print("Authorized");
        digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on green LED
        systemRunning = true;
    }

    // Prompt user to place finger for authentication if system not running
    if (!systemRunning) {
        lcd.setCursor(0, 0);
        lcd.print("Place Finger");
        int result = getFingerprintID();

        // Handle fingerprint verification result
        if (result >= 0) {
            if (systemAuthorized) {
                // Authorized user, stop the system
                lcd.clear();
                lcd.print("Place Finger");
                lcd.setCursor(0, 1);
                lcd.print("Press to Start");
                digitalWrite(RELAY_PIN, LOW);
                digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
                systemRunning = false;
                delay(2000);
            } else {
                // New authorized user, start the system
                lcd.clear();
                lcd.print("Authorized");
                lcd.setCursor(0, 1);
                lcd.print("System Starting");
                systemAuthorized = true;
                delay(2000);
            }
        } else {
            // Unauthorized user
            lcd.clear();
            lcd.print("Unauthorized");
            lcd.setCursor(0, 1);
            lcd.print("System Stopped");
            systemAuthorized = false;
            digitalWrite(RED_LED_PIN, HIGH);
            delay(2000);
        }
    }

    // Check for unauthorized attempts while system is running
    if (systemRunning && !systemAuthorized) {
        lcd.clear();
        lcd.print("Unauthorized");
        lcd.setCursor(0, 1);
        lcd.print("System Stopped");
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW); 
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);  // Turn on red LED
        delay(2000);
        digitalWrite(BUZZER_PIN, LOW);
        return;
    }
}

// Function to get fingerprint ID and verify
int getFingerprintID() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) return -1;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK) return -1;

    // Found a match
    Serial.print("Found ID #"); 
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); 
    Serial.println(finger.confidence);

    return finger.fingerID;
}

long measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2; // Calculate distance in cm

    return distance;
}

bool isHelmetWorn() {
    long distance = measureDistance();
    Serial.print("Distance: ");
    Serial.println(distance);
    return (distance < 10); // Assuming helmet is detected if within 10 cm
}
