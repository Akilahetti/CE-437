#include <SPI.h>
#include <Adafruit_PN532.h>

// Define Chip Select (CS) pins for each PN532 module
#define PN532_1_CS 5
#define PN532_2_CS 17
#define PN532_3_CS 16

#define PN532_SCK (18)
#define PN532_MOSI (23)
#define PN532_MISO (19)

// Create PN532 instances using SPI
// Adafruit_PN532 nfc1(PN532_1_CS);
// Adafruit_PN532 nfc2(PN532_2_CS);

Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_1_CS);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_2_CS);
Adafruit_PN532 nfc3(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_3_CS);

void checkPN532Firmware(Adafruit_PN532 &nfc, String name);
void readNFC(Adafruit_PN532 &nfc, String name);

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing PN532 modules over SPI...");

  // Initialize first PN532 module
  if (!nfc1.begin())
  {
    Serial.println("Error initializing PN532 #1");
    while (1)
      ;
  }
  delay(100);

  // Initialize second PN532 module
  if (!nfc2.begin())
  {
    Serial.println("Error initializing PN532 #2");
    while (1)
      ;
  }

  delay(100);

  if (!nfc3.begin())
  {
    Serial.println("Error initializing PN532 #2");
    while (1)
      ;
  }
  delay(100);

  // Get firmware versions
  checkPN532Firmware(nfc1, "PN532 #1");
  checkPN532Firmware(nfc2, "PN532 #2");
  checkPN532Firmware(nfc3, "PN532 #3");

  delay(100);
  while (!nfc1.setPassiveActivationRetries(0x9A))
  {
    Serial.println("Retrying setPassiveActivationRetries...");
    delay(100); // Small delay to prevent flooding
  }
  Serial.println("Successfully set Passive Activation Retries!");
  delay(100);
  nfc2.setPassiveActivationRetries(0x9A);
  delay(100);
  nfc3.setPassiveActivationRetries(0x9A);

  checkPN532Firmware(nfc1, "PN532 #1");
  checkPN532Firmware(nfc2, "PN532 #2");
  checkPN532Firmware(nfc3, "PN532 #3");
}

void loop()
{
  Serial.println("Checking for NFC tags...");

  // Read from PN532 #1
  readNFC(nfc1, "PN532 #1");

  delay(100);

  // Read from PN532 #2
  readNFC(nfc2, "PN532 #2");

  delay(100);

  // Read from PN532 #2
  readNFC(nfc3, "PN532 #3");

  delay(100);

  delay(2000); // Wait before checking again
}

void readNFC(Adafruit_PN532 &nfc, String name)
{
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the UID
  uint8_t uidLength;                     // Length of the UID

  Serial.print("Waiting for an NFC tag on ");
  Serial.println(name);

  // Check if a card is detected
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))
  {
    Serial.print(name);
    Serial.print(" detected an NFC tag! UID: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);
    }
    Serial.println();
  }
  else
  {
    Serial.print(name);
    Serial.println(" did not detect any NFC tag.");
  }
}

void checkPN532Firmware(Adafruit_PN532 &nfc, String name)
{
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.println(name + " failed to respond.");
    return;
  }

  Serial.println(name + " found!");
  Serial.print("Chip Version: ");
  Serial.print((versiondata >> 16) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((versiondata >> 8) & 0xFF, HEX);
  Serial.print("Supports: 0x");
  Serial.println(versiondata & 0xFF, HEX);

  delay(500);
}