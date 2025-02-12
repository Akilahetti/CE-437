#include <SPI.h>
#include <Adafruit_PN532.h>

// Define Chip Select (CS) pin for the PN532 module
#define PN532_CS 5

#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_MISO 19

#define A0 16
#define A1 4
// Create PN532 instance using SPI
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_CS);

bool tagPresent = false; // Track if a tag was previously detected

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing PN532 module...");
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);

    // Select slave 1
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);

    if (!nfc.begin())
    {
        Serial.println("Error initializing PN532");
        while (1)
            ;
    }

    nfc.wakeup();
    delay(100);

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

    delay(1000);
    Serial.println("First one done");

    // Select slave 1
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);

    if (!nfc.begin())
    {
        Serial.println("Error initializing PN532");
        while (1)
            ;
    }

    nfc.wakeup();
    delay(100);

    // Check firmware version
    versiondata = nfc.getFirmwareVersion();
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

uint8_t uid[7];
uint8_t uidLength;
int loop_counter = 0;

void loop()
{
    while (loop_counter < 100)
    {

        // Select slave 1
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);

        Serial.print("Checking for NFC tag try number: ");
        Serial.println(loop_counter);
        loop_counter++;

        // Check if a tag is detected
        if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100))
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
            tagPresent = false; // Reset tag detection state
            digitalWrite(BUILTIN_LED, LOW);
        }
    }
    loop_counter = 0;
    delay(2000);
    while (loop_counter < 100)
    {

        // Select slave 2
        digitalWrite(A0, HIGH);
        digitalWrite(A1, LOW);

        Serial.print("Checking for NFC tag try number: ");
        Serial.println(loop_counter);
        loop_counter++;

        // Check if a tag is detected
        if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100))
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
            tagPresent = false; // Reset tag detection state
            digitalWrite(BUILTIN_LED, LOW);
        }
    }

    loop_counter = 0;
    delay(5000);
}
