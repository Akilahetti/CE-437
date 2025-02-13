#include <SPI.h>
#include <Adafruit_PN532.h>
#include "PieceDefinitions.h"

#define PN532_CS 5
#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_MISO 19

#define A0 17
#define A1 16

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS);

uint8_t uid[7];
uint8_t uidLength;
const int numScans = 10;
const int numSlaves = 4;
int state[4] = {0,0,0,0};
String lastTagByte[4] = {"--", "--", "--", "--"}; // Stores the last byte of the detected tag UID

void selectSlave(int slave);
bool initializePN532();
void scanForTags(int curr_slave);
void printState();
int start_chip = 1;
int debug_mode = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing PN532 module...");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  for (int i = start_chip; i < numSlaves; i++)
  {
    selectSlave(i);
    if (!initializePN532())
    {
      Serial.print("Error initializing PN532 on slave ");
      Serial.println(i);
      while (1)
        ;
    }
  }
}

void loop()
{
  for (int i = start_chip; i < numSlaves; i++)
  {
    selectSlave(i);
    scanForTags(i);
    delay(100);
  }
  printState();
  delay(5000);
}

void selectSlave(int num)
{
  if (num < 1 || num > 4)
  {
    Serial.println("Invalid slave number");
    return;
  }

  num -= 1; // Adjust to zero-based index
  digitalWrite(A0, num & 0x01);
  digitalWrite(A1, (num >> 1) & 0x01);

}

bool initializePN532()
{
  if (!nfc.begin())
    return false;
  nfc.wakeup();
  delay(100);
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
    return false;
  Serial.print("PN532 initialized! Firmware Version: ");
  Serial.print((versiondata >> 16) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((versiondata >> 8) & 0xFF, HEX);
  return true;
}

void scanForTags(int curr_slave)
{
  for (int count = 0; count < numScans; count++)
  {
    if (debug_mode)
    {
      Serial.print("Checking for NFC tag try number: ");
      Serial.println(count);
    }


    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100))
    {
      if (debug_mode)
      {
        Serial.print("NFC Tag Detected! UID: ");
        for (uint8_t i = 0; i < uidLength; i++)
        {
          Serial.print(" 0x");
          Serial.print(uid[i], HEX);
        }
        Serial.println();
      }

      digitalWrite(LED_BUILTIN, HIGH);
      state[curr_slave-1] = 1;
      // Store the last byte of the UID as a string (in HEX format)
      lastTagByte[curr_slave - 1] = String(uid[uidLength - 1], HEX);
    }
    else
    {
      if(debug_mode)
      {
        Serial.println("No NFC tag detected.");
      }
      digitalWrite(LED_BUILTIN, LOW);
      state[curr_slave - 1] = 0;
      lastTagByte[curr_slave - 1] = "--"; // Reset if no tag is detected
    }
  }
}
void printState()
{
  Serial.println("\n===== NFC Reader States =====");
  Serial.println("Slave | State        | Last Tag Byte");
  Serial.println("----------------------------------");
  for (int i = 0; i < numSlaves; i++)
  {
    Serial.print("  ");
    Serial.print(i + 1);
    Serial.print("    |   ");
    Serial.print(state[i] ? "Tag Detected" : "No Tag");
    Serial.print("  |  ");
    // Convert lastTagByte to a uint8_t for lookup
    uint8_t tagValue = (uint8_t)strtoul(lastTagByte[i].c_str(), nullptr, 16);

    // Check if the tag corresponds to a chess piece
    if (pieceNames.find(tagValue) != pieceNames.end())
    {
      Serial.println(pieceNames.at(tagValue));
    }
    else
    {
      Serial.println("--");
    }
  }
  Serial.println("==================================\n");
}