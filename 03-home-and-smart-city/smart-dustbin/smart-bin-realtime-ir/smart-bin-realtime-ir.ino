#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart trash can"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

BlynkTimer timer;

#define echoPin D5
#define trigPin D6
#include<Servo.h>
Servo servo;
long duration;
int distance;
int binLevel = 0;

void SMESensor()
{
  int ir = digitalRead(D7);

  if (ir == HIGH)
  {
    servo.write(180);

    // Keep checking the IR sensor while it is active
    while (digitalRead(D7) == HIGH)
    {
      Blynk.virtualWrite(V2, 90);
      ultrasonic();
    }

    servo.write(0);
    Blynk.virtualWrite(V2, 0);
  }
  else
  {
    ultrasonic();
    delay(200);
  }
}

void ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  binLevel = map(distance, 18, 0, 0, 100);
  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, binLevel);
}

void setup()
{
  Serial.begin(9600);
  servo.attach(D2);
  pinMode(D7, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  delay(2000);
  timer.setInterval(1000L, SMESensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
