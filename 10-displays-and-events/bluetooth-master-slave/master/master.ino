//master 

#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // HC-05 TX, RX

#define ALCOHOL_SENSOR A0
#define WATER_SENSOR A1

void setup() {
    Serial.begin(9600);
    BT.begin(9600);
}

void loop() {
    int alcoholValue = analogRead(ALCOHOL_SENSOR);
    int waterValue = analogRead(WATER_SENSOR);

    Serial.print("Alcohol: ");
    Serial.print(alcoholValue);
    Serial.print(" | Water: ");
    Serial.println(waterValue);

    BT.print(alcoholValue);
    BT.print(",");
    BT.println(waterValue);

    delay(1000);
}