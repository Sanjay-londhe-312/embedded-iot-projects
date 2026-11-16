// Pin assignments
const int buttonPin = 6; // Button connected to pin 6
const int lightPin = 7;  // Light connected to pin 7

void setup() {
  // Set button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Set light pin as output
  pinMode(lightPin, OUTPUT);

  // Ensure the light is off initially
  digitalWrite(lightPin, LOW);
}

void loop() {
  // Read the button state
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (LOW because of pull-up resistor)
  if (buttonState == LOW) {
    // Turn on the light for 100ms
    digitalWrite(lightPin, HIGH);
    delay(100);
    digitalWrite(lightPin, LOW);
  }
}
