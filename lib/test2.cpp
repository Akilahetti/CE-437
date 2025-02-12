#include <SPI.h>
#include <Adafruit_PN532.h>

#define CS_INPUT 5   // GPIO5 - Simulated CS signal (Toggles HIGH/LOW)
#define SELECT_A0 17 // GPIO39 - A0
#define SELECT_A1 16 // GPIO16 - A1

int currentSelection = 0; // Toggle between 0 (Y0) and 1 (Y1)

void selectDevice(int device);

void setup()
{
    pinMode(CS_INPUT, OUTPUT);  // Simulated CS signal
    pinMode(SELECT_A0, OUTPUT); // A0 (LSB)
    pinMode(SELECT_A1, OUTPUT); // A1 (MSB)

    digitalWrite(CS_INPUT, LOW); // Start with CS low
    selectDevice(0);             // Start with Y0 active
}

void loop()
{
    digitalWrite(SELECT_A0, HIGH);
    digitalWrite(SELECT_A1, LOW);
    // Toggle CS input signal every 100ms (5 times per second)
    for (int i = 0; i < 10; i++)
    { // Repeat 10 times (1-second total duration)
        digitalWrite(CS_INPUT, !digitalRead(CS_INPUT));
        delay(100);
    }
}

// Function to select Y0 or Y1 by setting A0 and A1
void selectDevice(int device)
{
    digitalWrite(SELECT_A0, device & 0x01);        // Set A0 (Least Significant Bit)
    digitalWrite(SELECT_A1, (device >> 1) & 0x01); // Set A1 (Most Significant Bit)
}
