float temp;
float tempC = 0;
int buzzer = 13;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  noTone(buzzer);
}

void loop()
{
  temp = analogRead(A0);
  temp = ((temp * 5) / 1024);
  tempC = (temp - 0.5) * 100;
  Serial.print("Temperature = ");
  Serial.print(tempC);
  Serial.print(" C , ");
  if (tempC <= 20)
  {
    noTone(buzzer);
    delay(1000);
  }
  else
  {
    tone(13, 1000);
    delay(200);
  }

  // Working of Gas Sensor
  int gas = analogRead(A1);
  int limit = map(gas, 0, 1023, 0, 100);
  Serial.println(limit);

  if (limit > 70)
  {
    tone(13, 1000);
    delay(200);
  }
  else
  {
    noTone(buzzer);
    delay(1000);
  }
}