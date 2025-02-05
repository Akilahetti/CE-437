#include <SPI.h>
#include <Adafruit_PN532.h>

// Define CS pins for each PN532 sensor
#define CS_SENSOR_1 5
#define CS_SENSOR_2 17
#define CS_SENSOR_3 16

// // If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK (18)
#define PN532_MOSI (23)
#define PN532_SS (5)
#define PN532_MISO (19)

// Create PN532 objects for each sensor
// Adafruit_PN532 nfc1(CS_SENSOR_1);
// Adafruit_PN532 nfc2(CS_SENSOR_2);
// Adafruit_PN532 nfc3(CS_SENSOR_3);

Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_1);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_1);
Adafruit_PN532 nfc3(PN532_SCK, PN532_MISO, PN532_MOSI, CS_SENSOR_1);

// Store NFC sensor objects in an array for easier looping
Adafruit_PN532 *nfcSensors[] = {&nfc1, &nfc2, &nfc3};
uint8_t sensorPins[] = {CS_SENSOR_1, CS_SENSOR_2, CS_SENSOR_3};

void selectSensor(int index);
void deselectSensor(int index);
void resetRFField(int index);
void checkForTag(int index);

void setup()
{
    Serial.begin(115200);
    SPI.begin();

    // Initialize all NFC sensors
    for (int i = 0; i < 2; i++)
    {
        pinMode(sensorPins[i], OUTPUT);
        digitalWrite(sensorPins[i], HIGH); // Disable all sensors initially
        nfcSensors[i]->begin();
        uint32_t versiondata2 = nfcSensors[i]->getFirmwareVersion();
        if (!versiondata2)
        {
            Serial.print("Second PN53X board not found");
        }
        nfcSensors[i]->SAMConfig(); // Enable normal mode
    }
}

void loop()
{
    for (int i = 0; i < 2; i++)
    {
        selectSensor(i); // Enable the current sensor
        resetRFField(i); // Reset HALT state
        checkForTag(i);  // Poll for an NFC tag
        delay(10);
        deselectSensor(i); // Disable sensor before switching
        delay(1000);       // Short delay before moving to the next sensor
    }
}

void selectSensor(int index)
{
    digitalWrite(sensorPins[index], LOW); // Enable the specific NFC sensor
}

void deselectSensor(int index)
{
    digitalWrite(sensorPins[index], HIGH); // Disable the sensor after polling
}

void resetRFField(int index)
{
    // Turn OFF RF field
    uint8_t disableRF[] = {0xD4, 0x32, 0x01, 0x00};
    nfcSensors[index]->sendCommandCheckAck(disableRF, 4, 1000);
    delay(50); // Small delay for stability

    // Turn ON RF field
    uint8_t enableRF[] = {0xD4, 0x32, 0x01, 0x03};
    nfcSensors[index]->sendCommandCheckAck(enableRF, 4, 1000);
}

// void checkForTag(int index)
// {
//   uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
//   uint8_t uidLength;

//   if (nfcSensors[index]->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))
//   {
//     Serial.print("Sensor ");
//     Serial.print(index + 1);
//     Serial.print(" detected tag with UID: ");
//     for (uint8_t i = 0; i < uidLength; i++)
//     {
//       Serial.print(uid[i], HEX);
//       Serial.print(" ");
//     }
//     Serial.println();
//   }
// }

void checkForTag(int index)
{
    uint8_t pn532_packetbuffer[64];

    // Send InListPassiveTarget command (max 1 target, Type A)
    pn532_packetbuffer[0] = 0x4A; // InListPassiveTarget
    pn532_packetbuffer[1] = 1;    // MaxTg (1 target)
    pn532_packetbuffer[2] = 0;    // BrTy (ISO14443A)

    if (!nfcSensors[index]->sendCommandCheckAck(pn532_packetbuffer, 3, 100))
    {
        Serial.print("Sensor ");
        Serial.print(index + 1);
        Serial.println(": No tag detected.");
        return; // Move to next sensor
    }

    // Read the response
    nfcSensors[index]->readdata(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    // Validate response structure
    if (pn532_packetbuffer[6] != 0x4B || pn532_packetbuffer[7] == 0)
    {
        Serial.print("Sensor ");
        Serial.print(index + 1);
        Serial.println(": No valid tag found.");
        return; // No tag detected, move to next sensor
    }

    // Extract UID length and UID
    uint8_t uidLength = pn532_packetbuffer[12];
    Serial.print("Sensor ");
    Serial.print(index + 1);
    Serial.print(" detected tag with UID: ");

    for (uint8_t i = 0; i < uidLength; i++)
    {
        Serial.print(pn532_packetbuffer[13 + i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

// /**************************************************************************/
// /*!
//     @file     readntag203.pde
//     @author   KTOWN (Adafruit Industries)
//     @license  BSD (see license.txt)

//     This example will wait for any NTAG203 or NTAG213 card or tag,
//     and will attempt to read from it.

//     This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
//     This library works with the Adafruit NFC breakout
//       ----> https://www.adafruit.com/products/364

//     Check out the links above for our tutorials and wiring diagrams
//     These chips use SPI or I2C to communicate.

//     Adafruit invests time and resources providing this open source code,
//     please support Adafruit and open-source hardware by purchasing
//     products from Adafruit!
// */
// /**************************************************************************/
// #include <Wire.h>
// #include <SPI.h>
// #include <Adafruit_PN532.h>

// // If using the breakout with SPI, define the pins for SPI communication.
// #define PN532_SCK (18)
// #define PN532_MOSI (23)
// #define PN532_SS (5)
// #define PN532_MISO (19)

// // If using the breakout or shield with I2C, define just the pins connected
// // to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
// #define PN532_IRQ (2)
// #define PN532_RESET (3) // Not connected by default on the NFC Shield

// // Uncomment just _one_ line below depending on how your breakout or shield
// // is connected to the Arduino:

// // Use this line for a breakout with a software SPI connection (recommended):
// Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// // Use this line for a breakout with a hardware SPI connection.  Note that
// // the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// // hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// // SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
// // Adafruit_PN532 nfc(PN532_SS);

// // Or use this line for a breakout or shield with an I2C connection:
// // Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// void setup(void)
// {
//   Serial.begin(115200);
//   while (!Serial)
//     delay(10); // for Leonardo/Micro/Zero

//   Serial.println("Hello!");

//   nfc.begin();

//   uint32_t versiondata = nfc.getFirmwareVersion();
//   if (!versiondata)
//   {
//     Serial.print("Didn't find PN53x board");
//     while (1)
//       ; // halt
//   }
//   // Got ok data, print it out!
//   Serial.print("Found chip PN5");
//   Serial.println((versiondata >> 24) & 0xFF, HEX);
//   Serial.print("Firmware ver. ");
//   Serial.print((versiondata >> 16) & 0xFF, DEC);
//   Serial.print('.');
//   Serial.println((versiondata >> 8) & 0xFF, DEC);

//   Serial.println("Waiting for an ISO14443A Card ...");
// }

// void loop(void)
// {
//   uint8_t success;
//   uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
//   uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

//   // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
//   // the UID, and uidLength will indicate the size of the UUID (normally 7)
//   success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

//   if (success)
//   {
//     // Display some basic information about the card
//     Serial.println("Found an ISO14443A card");
//     Serial.print("  UID Length: ");
//     Serial.print(uidLength, DEC);
//     Serial.println(" bytes");
//     Serial.print("  UID Value: ");
//     nfc.PrintHex(uid, uidLength);
//     Serial.println("");

//     if (uidLength == 7)
//     {
//       uint8_t data[32];

//       // We probably have an NTAG2xx card (though it could be Ultralight as well)
//       Serial.println("Seems to be an NTAG2xx tag (7 byte UID)");

//       // NTAG2x3 cards have 39*4 bytes of user pages (156 user bytes),
//       // starting at page 4 ... larger cards just add pages to the end of
//       // this range:

//       // See: http://www.nxp.com/documents/short_data_sheet/NTAG203_SDS.pdf

//       // TAG Type       PAGES   USER START    USER STOP
//       // --------       -----   ----------    ---------
//       // NTAG 203       42      4             39
//       // NTAG 213       45      4             39
//       // NTAG 215       135     4             129
//       // NTAG 216       231     4             225

//       for (uint8_t i = 0; i < 42; i++)
//       {
//         success = nfc.ntag2xx_ReadPage(i, data);

//         // Display the current page number
//         Serial.print("PAGE ");
//         if (i < 10)
//         {
//           Serial.print("0");
//           Serial.print(i);
//         }
//         else
//         {
//           Serial.print(i);
//         }
//         Serial.print(": ");

//         // Display the results, depending on 'success'
//         if (success)
//         {
//           // Dump the page data
//           nfc.PrintHexChar(data, 4);
//         }
//         else
//         {
//           Serial.println("Unable to read the requested page!");
//         }
//       }
//     }
//     else
//     {
//       Serial.println("This doesn't seem to be an NTAG203 tag (UUID length != 7 bytes)!");
//     }

//     // Wait a bit before trying again
//     Serial.println("\n\nSend a character to scan another tag!");
//     Serial.flush();
//     while (!Serial.available())
//       ;
//     while (Serial.available())
//     {
//       Serial.read();
//     }
//     Serial.flush();
//   }
// }