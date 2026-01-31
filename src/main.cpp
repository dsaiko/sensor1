#include "board.h"
#include "display.h"
#include "info.h"
#include "led.h"
#include "neoled.h"

#include <Arduino.h>
#include <Wire.h>

constexpr uint32_t kSerialBaudRate = 115200;

// ESP32-S3 DevKitC-1: GPIO8/9 are usable for I2C.
constexpr int kI2cSdaPin = 9;
constexpr int kI2cSclPin = 8;
constexpr uint32_t kI2cClockHz = 100000; // Standard-mode I2C (100 kHz)

void setup()
{
    board::disableRadio();
    board::setCPUFrequency();

    Serial.begin(kSerialBaudRate);
    delay(50); // Give USB-serial a moment to come up.

    // Bring up I2C before initializing any I2C peripherals.
    // Note: internal pullups are weak; prefer external 4.7k pullups to 3.3V.
    pinMode(kI2cSdaPin, INPUT_PULLUP);
    pinMode(kI2cSclPin, INPUT_PULLUP);
    Wire.begin(kI2cSdaPin, kI2cSclPin);
    Wire.setClock(kI2cClockHz);
    delay(10);

    // Initialize peripherals.
    neoled::off();
    led::init();
    display::init();

    // Optional: wait for a serial monitor to attach.
    delay(1000);

    board::printInfo();
}

void loop()
{
    led::run();
    display::draw();

    // Demo: toggle error state and update values.
    const int64_t uptimeMs = board::now();
    if (uptimeMs > 10000 && uptimeMs < 20000)
    {
        led::setErrorState(true);
    }
    else if (uptimeMs > 20000)
    {
        led::setErrorState(false);
        display::setValues(23, 45, 678);
    }

    delay(100);
}