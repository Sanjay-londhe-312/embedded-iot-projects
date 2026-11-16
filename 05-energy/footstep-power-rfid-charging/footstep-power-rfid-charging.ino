#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define RELAY_PIN 8 // Change this to the pin connected to the relay module

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin();        // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is initially turned off
}

void loop()
{
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // Show UID on the serial monitor
  Serial.print("UID tag:");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Check if the UID is authorized (replace with your authorized UIDs)
  if (isAuthorized(mfrc522.uid))
  {
    Serial.println("User authorized. Charging started.");
    activateRelay(true); // Turn on the relay
    // Implement charging logic here, e.g., provide power to the powerbank
  }
  else
  {
    Serial.println("Unauthorized access. Charging not allowed.");
    activateRelay(false); // Turn off the relay
  }

  delay(2000); // Delay to avoid reading the same card repeatedly
}

bool isAuthorized(MFRC522::Uid uid)
{
  // Replace with the UIDs of authorized cards
  // Example: "E3 34 38 FA", "81 93 40 43", "91 69 3E 43"
  String authorizedUIDs[] = {"E3 34 38 FA","F3 6F 72 EC","81 93 40 43", "91 69 3E 43"};

  String cardUID = "";
  for (byte i = 0; i < uid.size; i++)
  {
    cardUID.concat(String(uid.uidByte[i] < 0x10 ? " 0" : " "));
    cardUID.concat(String(uid.uidByte[i], HEX));
  }

  cardUID.toUpperCase();

  for (int i = 0; i < sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]); i++)
  {
    if (cardUID.substring(1) == authorizedUIDs[i])
    {
      return true;
    }
  }

  return false;
}

void activateRelay(bool state)
{
  digitalWrite(RELAY_PIN, state ? HIGH : LOW); // Activate or deactivate the relay
}
