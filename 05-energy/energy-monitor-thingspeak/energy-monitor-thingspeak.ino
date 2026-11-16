
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int int_busvoltage = 0;
int x, y, z;
float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;
unsigned long previousMillis_V = 0;
unsigned long previousMillis_I = 0;
unsigned long previousMillis_P = 0;
const int Field_number_1 = 1;
const int Field_number_2 = 2;
const int Field_number_3 = 3;
bool measure_V = true;
bool measure_I = false;
bool measure_P = false;

//------- WI-FI details ----------//
char ssid[] = "YOUR_WIFI_NAME"; //SSID here
char pass[] = "YOUR_WIFI_PASSWORD"; // Password here
//--------------------------------//

//----------- Channel details ----------------//
unsigned long Channel_ID = 123456; // Your Channel ID
const char *WriteAPIKey = "ABC123CDEF456"; //Your write API key
//-------------------------------------------//

WiFiClient client;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IoT Based Energy");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring Systm");
  delay(2000);
  
  if (!ina219.begin())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAILED TO FIND");
    lcd.setCursor(0, 1);
    lcd.print("INA219 MODULE");
    while (1)
    {
      delay(10);
    }
  }
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  internet();
  measure();
  upload();
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CONNECTING TO");
    lcd.setCursor(0, 1);
    lcd.print("Wi-Fi NETWORK...");
    delay(5000);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
  }
}

void measure()
{
  busvoltage = ina219.getBusVoltage_V();
  int_busvoltage = busvoltage * 1000;
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(busvoltage);
  lcd.setCursor(9, 1);
  lcd.print("mA:");
  if (current_mA < 3)
  {
    lcd.print("000");
  }
  else if (current_mA < 10)
  {
    lcd.print("00");
    lcd.print((int)current_mA);
  }
  else if (current_mA < 100)
  {
    lcd.print('0');
    lcd.print((int)current_mA);
  }
  else
  {
    lcd.print((int)current_mA);
  }
  
  lcd.setCursor(0, 0);
  lcd.print("POWER(mW):");
  lcd.print((int)power_mW);
  delay(2000);
}

void upload()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_V >= 15000 && measure_V)
  {
    previousMillis_V = currentMillis;
    x = ThingSpeak.writeField(Channel_ID, Field_number_1, busvoltage, WriteAPIKey);
    if (x == 200)
    {
      measure_V = false;
      measure_I = true;
    }
  }

  if (currentMillis - previousMillis_I >= 15000 && measure_I)
  {
    previousMillis_I = currentMillis;
    y = ThingSpeak.writeField(Channel_ID, Field_number_2, current_mA, WriteAPIKey);
    if (y == 200)
    {
      measure_I = false;
      measure_P = true;
    }
  }

  if (currentMillis - previousMillis_P >= 15000 && measure_P)
  {
    previousMillis_P = currentMillis;
    z = ThingSpeak.writeField(Channel_ID, Field_number_3, power_mW, WriteAPIKey);
    if (z == 200)
    {
      measure_P = false;
      measure_V = true;
    }
  }
}
