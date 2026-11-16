#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
int pos = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16x2 display

const byte rows = 4;
const byte cols = 3;
char key[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {1, 2, 3, 4};
byte colPins[cols] = {5, 6, 7};
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);

char* password = "YOUR_WIFI_PASSWORD";
int currentposition = 0;
int redled = 10;
int greenled = 11;
int buzz = 8;
int invalidcount = 12;

void setup() {
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on backlight

  Serial.begin(9600);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(buzz, OUTPUT);
  myservo.attach(9);

  displayscreen();
}

void loop() {
  if (currentposition == 0) {
    displayscreen();
  }

  int l;
  char code = keypad.getKey();
  if (code != NO_KEY) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD:");
    lcd.setCursor(7, 1);
    lcd.print(" ");
    lcd.setCursor(7, 1);
    for (l = 0; l <= currentposition; ++l) {
      lcd.print("*");
      keypress();
    }

    if (code == password[currentposition]) {
      ++currentposition;
      if (currentposition == 4) {
        unlockdoor();
        currentposition = 0;
      }
    } else {
      ++invalidcount;
      incorrect();
      currentposition = 0;
    }

    if (invalidcount == 5) {
      ++invalidcount;
      torture1();
    }
    if (invalidcount == 8) {
      torture2();
    }
  }
}

void unlockdoor() {
  delay(900);
  lcd.setCursor(0, 0);
  lcd.println(" ");
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.println("Get in!!");
  lcd.setCursor(15, 1);
  lcd.println(" ");
  lcd.setCursor(16, 1);
  lcd.println(" ");
  lcd.setCursor(14, 1);
  lcd.println(" ");
  lcd.setCursor(13, 1);
  lcd.println(" ");
  unlockbuzz();

  for (pos = 90; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(5);
  }
  delay(2000);

  delay(1000);
  counterbeep();

  delay(1000);

  for (pos = 0; pos <= 90; pos += 5) {
    myservo.write(pos);
    delay(15);
  }

  currentposition = 0;
  lcd.clear();
  displayscreen();
}

void incorrect() {
  delay(500);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CODE");
  lcd.setCursor(6, 0);
  lcd.print("INCORRECT");
  lcd.setCursor(15, 1);
  lcd.println(" ");
  lcd.setCursor(4, 1);
  lcd.println("GET AWAY!!!");

  lcd.setCursor(13, 1);
  lcd.println(" ");
  Serial.println("CODE INCORRECT YOU ARE UNAUTHORIZED");
  digitalWrite(redled, HIGH);
  digitalWrite(buzz, HIGH);
  delay(3000);
  lcd.clear();
  digitalWrite(redled, LOW);
  digitalWrite(buzz, LOW);
  displayscreen();
}

void clearscreen() {
  lcd.setCursor(0, 0);
  lcd.println(" ");
  lcd.setCursor(0, 1);
  lcd.println(" ");
  lcd.setCursor(0, 2);
  lcd.println(" ");
  lcd.setCursor(0, 3);
  lcd.println(" ");
}

void keypress() {
  digitalWrite(buzz, HIGH);
  delay(50);
  digitalWrite(buzz, LOW);
}

void displayscreen() {
  lcd.setCursor(0, 0);
  lcd.println("*ENTER THE CODE*");
  lcd.setCursor(1, 1);
  lcd.println("TO _/_ (OPEN)!!");
}

void armservo() {
  for (pos = 180; pos <= 180; pos += 50) {
    myservo.write(pos);
    delay(5);
  }
  delay(5000);

  for (pos = 180; pos >= 0; pos -= 50) {
    myservo.write(pos);
  }
}

void unlockbuzz() {
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(200);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
}

void counterbeep() {
  delay(1200);
  lcd.clear();
  digitalWrite(buzz, HIGH);

  lcd.setCursor(2, 15);
  lcd.println(" ");
  lcd.setCursor(2, 14);
  lcd.println(" ");
  lcd.setCursor(2, 0);
  delay(200);
  lcd.println("GET IN WITHIN:::");

  lcd.setCursor(4, 1);
  lcd.print("5");
  delay(200);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);

  digitalWrite(buzz, HIGH);
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  lcd.setCursor(4, 1); //2
  lcd.print("4");
  delay(100);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);

  digitalWrite(buzz, HIGH);
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  lcd.setCursor(4, 1); //3
  lcd.print("3");
  delay(100);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);

  digitalWrite(buzz, HIGH);
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  lcd.setCursor(4, 1); //4
  lcd.print("2");
  delay(100);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);

  digitalWrite(buzz, HIGH);
  lcd.setCursor(4, 1);
  lcd.print("1");
  delay(100);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN::");
  digitalWrite(buzz, LOW);
  delay(1000);

  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("RE-LOCKING");
  delay(500);
  lcd.setCursor(12, 0);
  lcd.print(".");
  delay(500);
  lcd.setCursor(13, 0);
  lcd.print(".");
  delay(500);
  lcd.setCursor(14, 0);
  lcd.print(".");
  delay(400);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("LOCKED!");
  delay(440);
}

void torture1() {
  delay(1000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("WAIT FOR ");
  lcd.setCursor(5, 1);
  lcd.print("15 SECONDS");
  digitalWrite(buzz, HIGH);
  delay(15000);
  digitalWrite(buzz, LOW);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LOL..");
  lcd.setCursor(1, 1);
  lcd.print(" HOW WAS THAT??");
  delay(3500);
  lcd.clear();
}

void torture2() {
  delay(1000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("WAIT FOR ");
  lcd.setCursor(5, 1);
  lcd.print("20 SECONDS");
  digitalWrite(buzz, HIGH);
  delay(20000);
  digitalWrite(buzz, LOW);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LOL...");
  lcd.setCursor(1, 1);
  lcd.print("HOW WAS THAT??");
  delay(3500);
  lcd.clear();
}
