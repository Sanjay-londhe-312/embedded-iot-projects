// Define relay pins
//const int relay1 = 2;
//const int relay2 = 3;
const int relay3 = D4;

// Variables to hold relay states
//bool state1 = false;
//bool state2 = false;
bool state3 = false;

void setup() {
  // Set relay pins as OUTPUT
  //pinMode(relay1, OUTPUT);
  //pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  // Start Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Toggle states
  //state1 = !state1;
  //state2 = !state2;
  state3 = !state3;

  // Write states to relays
  //digitalWrite(relay1, state1 ? HIGH : LOW);
  //digitalWrite(relay2, state2 ? HIGH : LOW);
  digitalWrite(relay3, state3 ? HIGH : LOW);

  // Print status to Serial Monitor
  /*Serial.println("Relay States:");
  Serial.print("Relay 1: ");
  Serial.println(state1 ? "ON" : "OFF");
  Serial.print("Relay 2: ");
  Serial.println(state2 ? "ON" : "OFF");*/
  Serial.print("Relay 3: ");
  Serial.println(state3 ? "ON" : "OFF");
  Serial.println("----------------------");

  // Wait for 1 second
  delay(1000);
}
