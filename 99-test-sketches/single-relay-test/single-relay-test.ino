// Define the relay pin
const int relayPin = 2; // You can change this to any other digital pin

void setup() {
  // Set the relay pin as output
  pinMode(relayPin, OUTPUT);
  // Initialize the relay to be off
  digitalWrite(relayPin, LOW);

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Print a message to the serial monitor
  Serial.println("Enter ON to turn the motor on and OFF to turn the motor off.");
}

void loop() {
  // Check if data is availableON to read from the serial port
  if (Serial.available() > 0) {
    // Read the incoming data
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any leading or trailing whitespace

    // Check the command and turn the motor on or off accordingly
    if (command.equalsIgnoreCase("ON")) {
      digitalWrite(relayPin, HIGH); // Turn the relay on
      Serial.println("Motor is ON");
    } else if (command.equalsIgnoreCase("OFF")) {
      digitalWrite(relayPin, LOW); // Turn the relay off
      Serial.println("Motor is OFF");
    } else {
      Serial.println("Invalid command. Please enter ON or OFF.");
    }
  }
}
