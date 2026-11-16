#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "YOUR_WIFI_NAME"    
#define WIFI_PASS         "YOUR_WIFI_PASSWORD"
#define APP_KEY           "YOUR_SINRIC_APP_KEY"
#define APP_SECRET        "YOUR_SINRIC_APP_SECRET"

#define DEVICE_ID_1   "YourDeviceID1"
#define DEVICE_ID_2   "YOUR_DEVICE_ID" //bedroomlight
#define DEVICE_ID_3   "YOUR_DEVICE_ID" //roomlight
#define DEVICE_ID_4   "YOUR_DEVICE_ID" //kichen

#define SWITCH_PIN_1 5  //D1
#define SWITCH_PIN_2 4  //D2
#define SWITCH_PIN_3 14 //D5
#define SWITCH_PIN_4 12 //D6

#define BULB_PIN_1   0  // Change to the GPIO pin connected to Bulb 1
#define BULB_PIN_2   2  // Change to the GPIO pin connected to Bulb 2
#define BULB_PIN_3   13 // Change to the GPIO pin connected to Bulb 3
#define BULB_PIN_4   15 // Change to the GPIO pin connected to Bulb 4

#define WIFI_LED_PIN 16   //D0
#define BAUD_RATE    9600
#define DEBOUNCE_TIME 250

typedef struct {
  int switchPin;
  int bulbPin;
} DeviceConfig;

std::map<String, DeviceConfig> devices = {
    {DEVICE_ID_1, {SWITCH_PIN_1, BULB_PIN_1}},
    {DEVICE_ID_2, {SWITCH_PIN_2, BULB_PIN_2}},
    {DEVICE_ID_3, {SWITCH_PIN_3, BULB_PIN_3}},
    {DEVICE_ID_4, {SWITCH_PIN_4, BULB_PIN_4}}
};

typedef struct {
  String deviceId;
  bool lastSwitchState;
  unsigned long lastSwitchChange;
} SwitchConfig;

typedef struct {
  String deviceId;
  bool lastBulbState;
} BulbConfig;

std::map<int, SwitchConfig> switches;
std::map<int, BulbConfig> bulbs;

void setupSwitches() {
  for (auto &device : devices) {
    SwitchConfig switchConfig;
    switchConfig.deviceId = device.first;
    switchConfig.lastSwitchChange = 0;
    switchConfig.lastSwitchState = true;
    
    int switchPin = device.second.switchPin;
    switches[switchPin] = switchConfig;
    pinMode(switchPin, INPUT_PULLUP);
  }
}

void setupBulbs() {
  for (auto &device : devices) {
    BulbConfig bulbConfig;
    bulbConfig.deviceId = device.first;
    bulbConfig.lastBulbState = false; // Change the initial state as needed
    
    int bulbPin = device.second.bulbPin;
    bulbs[bulbPin] = bulbConfig;
    pinMode(bulbPin, OUTPUT);
    digitalWrite(bulbPin, HIGH);
  }
}

bool onPowerState(String deviceId, bool &state) {
  Serial.printf("%s: %s\r\n", deviceId.c_str(), state ? "on" : "off");

  if (deviceId == DEVICE_ID_1) {
    int bulbPin = devices[DEVICE_ID_1].bulbPin;
    digitalWrite(bulbPin, state);
  } 
  // Add similar blocks for other devices if needed

  return true;
}

void handleSwitches() {
  unsigned long actualMillis = millis();
  for (auto &switchEntry : switches) {
    unsigned long lastSwitchChange = switchEntry.second.lastSwitchChange;
    if (actualMillis - lastSwitchChange > DEBOUNCE_TIME) {
      int switchPin = switchEntry.first;
      bool lastSwitchState = switchEntry.second.lastSwitchState;
      bool switchState = digitalRead(switchPin);
      if (switchState != lastSwitchState) {
        switchEntry.second.lastSwitchChange = actualMillis;
        String deviceId = switchEntry.second.deviceId;
        SinricProSwitch &mySwitch = SinricPro[deviceId];
        mySwitch.sendPowerStateEvent(!lastSwitchState);
        switchEntry.second.lastSwitchState = switchState;
      }
    }
  }
}

void handleBulbs() {
  for (auto &bulbEntry : bulbs) {
    int bulbPin = bulbEntry.first;
    bool lastBulbState = bulbEntry.second.lastBulbState;
    bool bulbState = digitalRead(bulbPin);

    if (bulbState != lastBulbState) {
      bulbEntry.second.lastBulbState = bulbState;
      String deviceId = bulbEntry.second.deviceId;
      SinricProSwitch &mySwitch = SinricPro[deviceId];
      mySwitch.sendPowerStateEvent(bulbState);
    }
  }
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  digitalWrite(WIFI_LED_PIN, LOW);
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  for (auto &device : devices) {
    const char *deviceId = device.first.c_str();
    SinricProSwitch &mySwitch = SinricPro[deviceId];
    mySwitch.onPowerState(onPowerState);
  }

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(WIFI_LED_PIN, OUTPUT);
  digitalWrite(WIFI_LED_PIN, HIGH);
  setupSwitches();
  setupBulbs();
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
  handleSwitches();
  handleBulbs();
}
