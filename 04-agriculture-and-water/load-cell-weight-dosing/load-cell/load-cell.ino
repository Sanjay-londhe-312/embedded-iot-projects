#include "HX711.h"

HX711 scale;

// Using Teensy 3.5 digital pins 40 = DT, 41 = SCK
const int LOADCELL_DOUT_PIN = 40;
const int LOADCELL_SCK_PIN = 41;

float tareOffset = 0;
float targetWeight = 0;
String input = "";
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 300; // Update every 500 ms

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(-1674991.00);  // TODO: Set your calibration factor here
  scale.tare();       // Tare the initial load

  Serial.println("Weight System is ready");
  Serial.println("Commands: zero | set <kg> | reset");
}

void loop() {
  float weight = scale.get_units(5) - tareOffset;

  // Print weight continuously every 500 ms
  if (millis() - lastPrintTime > printInterval) {
    Serial.print("Weight: ");
    Serial.print(weight, 2);
    Serial.print(" kg | Target: ");
    Serial.print(targetWeight, 2);
    Serial.println(" kg");
    lastPrintTime = millis();
  }

  // Handle serial input
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') {
      if (input.length() > 0) {
        handleCommand(input);
        input = "";
      }
    } else {
      input += ch;
    }
  }
}

void handleCommand(String cmd) {
  cmd.trim();

  if (cmd.equalsIgnoreCase("zero")) {
    tareOffset = scale.get_units(5);
    Serial.println("[✔] Tare complete. Flask weight removed.");
  }
  else if (cmd.startsWith("set ")) {
    float val = cmd.substring(4).toFloat();
    if (val > 0) {
      targetWeight = val;
      Serial.print("[✔] Target weight set to ");
      Serial.print(targetWeight, 2);
      Serial.println(" kg.");
    } else {
      Serial.println("[✘] Invalid value. Use: set <number>");
    }
  }
  else if (cmd.equalsIgnoreCase("reset")) {
    tareOffset = 0;
    targetWeight = 0;
    Serial.println("[✔] System reset.");
  }
  else {
    Serial.println("[✘] Unknown command. Use: zero | set <kg> | reset");
  }
}
