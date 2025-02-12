#include <SPI.h>
#include <Adafruit_PN532.h>

// Define Chip Select (CS) pin for the PN532 module
#define PN532_CS_1 5
#define PN532_CS_2 16

#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_MISO 19

// Create PN532 instance using SPI
// Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS_1);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS_2);
Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS_1);

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Initializing PN532 module...");
    if (!nfc1.begin())
    {
        Serial.println("Error initializing PN532");
        while (1)
            ;
    }
    nfc1.wakeup();
    delay(100);
    // Check firmware version
    uint32_t versiondata = nfc1.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.println("PN532 failed to respond.");
        while (1)
            ;
    }
    Serial.println("PN532 1  initialized!");
    Serial.print("Firmware Version: ");
    Serial.print((versiondata >> 16) & 0xFF, HEX);
    Serial.print(".");
    Serial.println((versiondata >> 8) & 0xFF, HEX);

    delay(1000);
    Serial.println("Initializing PN532 module...");
    if (!nfc2.begin())
    {
        Serial.println("Error initializing PN532");
        while (1)
            ;
    }
    nfc2.wakeup();
    delay(100);
    // Check firmware version
    uint32_t versiondata2 = nfc2.getFirmwareVersion();
    if (!versiondata2)
    {
        Serial.println("PN532 failed to respond.");
        while (1)
            ;
    }
    Serial.println("PN532 1  initialized!");
    Serial.print("Firmware Version: ");
    Serial.print((versiondata2 >> 16) & 0xFF, HEX);
    Serial.print(".");
    Serial.println((versiondata2 >> 8) & 0xFF, HEX);
    delay(1000);
}

uint8_t uid[7];
uint8_t uidLength;
int loop_counter = 0;

void loop()
{

    Serial.print("Checking for NFC tag try number: ");
    Serial.println(loop_counter);
    loop_counter++;

    // Check if a tag is detected
    if (nfc1.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 300))
    {
        Serial.print("NFC Tag Detected! UID: ");
        for (uint8_t i = 0; i < uidLength; i++)
        {
            Serial.print(" 0x");
            Serial.print(uid[i], HEX);
        }
        Serial.println();

        digitalWrite(BUILTIN_LED, HIGH);
    }
    else
    {

        Serial.println("No NFC tag detected.");
        digitalWrite(BUILTIN_LED, LOW);
    }

    if (nfc2.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 300))
    {
        Serial.print("NFC Tag Detected! UID: ");
        for (uint8_t i = 0; i < uidLength; i++)
        {
            Serial.print(" 0x");
            Serial.print(uid[i], HEX);
        }
        Serial.println();

        // digitalWrite(BUILTIN_LED, HIGH);
    }
    else
    {

        Serial.println("No NFC tag detected.");
        // digitalWrite(BUILTIN_LED, LOW);
    }
}
