#include <SPI.h>
#include <Adafruit_PN532.h>
#include "string.h"

// Define Chip Select (CS) pin for the PN532 module
#define CS_SENSOR_1 5
#define CS_SENSOR_2 17
#define CS_SENSOR_3 16

#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_MISO 19

// Create PN532 instance using SPI
Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_1);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_2);
Adafruit_PN532 nfc3(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_3);

Adafruit_PN532 *nfcSensors[] = {&nfc1, &nfc2, &nfc3};
uint8_t sensorPins[] = {CS_SENSOR_1, CS_SENSOR_2, CS_SENSOR_3};
int state[3] = {0, 0, 0};

unsigned long lastReadTime = 0;
bool tagPresent = false; // Track if a tag was previously detected

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing PN532 module...");
    pinMode(LED_BUILTIN, OUTPUT);
    for (int i = 0; i < 2; i++)
    {
        if (!nfcSensors[i]->begin())
        {
            Serial.println("Error initializing PN532");
            while (1)
                ;
        }
        nfcSensors[i]->wakeup();
        delay(100);
        // Check firmware version
        uint32_t versiondata = nfcSensors[i]->getFirmwareVersion();
        if (!versiondata)
        {
            Serial.println("PN532 failed to respond.");
            while (1)
                ;
        }

        Serial.print("Board ");
        Serial.print(i);
        Serial.print("PN532 initialized!");
        Serial.print("Firmware Version: ");
        Serial.print((versiondata >> 16) & 0xFF, HEX);
        Serial.print(".");
        Serial.println((versiondata >> 8) & 0xFF, HEX);
    }
}

uint8_t uid[7];
uint8_t uidLength;
int loop_counter = 0;

void loop()
{

    // Serial.print("Checking for NFC tag try number: ");
    // Serial.println(loop_counter);

    // Check if a tag is detected

    for (int i = 0; i < 2; i++)
    {
        if (nfcSensors[i]->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100))
        {
            Serial.print("NFC Tag Detected! UID: ");
            for (uint8_t i = 0; i < uidLength; i++)
            {
                Serial.print(" 0x");
                Serial.print(uid[i], HEX);
            }
            Serial.println();
            state[i] = 1;
        }
        else
        {

            // Serial.println("No NFC tag detected.");
            state[i] = 0;
        }
    }
    Serial.print(loop_counter);
    Serial.print(":\t");
    Serial.print(state[0]);
    Serial.print("\t");
    Serial.print(state[1]);
    Serial.print("\t");
    Serial.println(state[2]);
    loop_counter++;
}
