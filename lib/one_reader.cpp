#include <SPI.h>
#include <Adafruit_PN532.h>

// Define Chip Select (CS) pin for the PN532 module
#define PN532_CS 5

#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_MISO 19

// Create PN532 instance using SPI
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS);

unsigned long lastReadTime = 0;
bool tagPresent = false; // Track if a tag was previously detected

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing PN532 module...");

    if (!nfc.begin())
    {
        Serial.println("Error initializing PN532");
        while (1)
            ;
    }

    // Check firmware version
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.println("PN532 failed to respond.");
        while (1)
            ;
    }

    Serial.println("PN532 initialized!");
    Serial.print("Firmware Version: ");
    Serial.print((versiondata >> 16) & 0xFF, HEX);
    Serial.print(".");
    Serial.println((versiondata >> 8) & 0xFF, HEX);
}

void loop()
{
    unsigned long currentTime = millis();

    if (currentTime - lastReadTime >= 1000)
    {
        lastReadTime = currentTime;

        Serial.println("Checking for NFC tag...");

        uint8_t uid[7];
        uint8_t uidLength;

        // Check if a tag is detected
        if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))

        {
            if (!tagPresent) // Only print when a new tag is detected
            {
                Serial.print("NFC Tag Detected! UID: ");
                for (uint8_t i = 0; i < uidLength; i++)
                {
                    Serial.print(" 0x");
                    Serial.print(uid[i], HEX);
                }
                Serial.println();
                tagPresent = true; // Mark that a tag is present
            }
        }
        else
        {
            if (tagPresent) // Only print when a tag is removed
            {
                Serial.println("No NFC tag detected.");
                tagPresent = false; // Reset tag detection state
            }
        }
    }
}
