#include "board.h"
#include "display.h"
#include "info.h"
#include "led.h"
#include "neoled.h"
#include "pins.h"
#include "sensor.h"

#include <Arduino.h>
#include <Wire.h>

constexpr uint32_t kSerialBaudRate = 115200;

// ESP32-S3 DevKitC-1: GPIO8/9 are usable for I2C.
constexpr int kI2cSdaPin = pins::kI2cSda;
constexpr int kI2cSclPin = pins::kI2cScl;
constexpr uint32_t kI2cClockHz = 100000; // Standard-mode I2C (100 kHz)

constexpr int kMaxCO2ppm = 1500;

// Power tuning
#ifndef POWER_LOOP_INTERVAL_MS
#define POWER_LOOP_INTERVAL_MS 1000
#endif

#ifndef POWER_ENABLE_LIGHT_SLEEP
#define POWER_ENABLE_LIGHT_SLEEP 1
#endif

#ifndef POWER_AVOID_SLEEP_WHEN_SERIAL
#define POWER_AVOID_SLEEP_WHEN_SERIAL 1
#endif

constexpr uint32_t kLoopIntervalMs = POWER_LOOP_INTERVAL_MS;
constexpr bool kEnableLightSleepBetweenLoops = (POWER_ENABLE_LIGHT_SLEEP != 0);
constexpr bool kAvoidSleepWhenSerialConnected = (POWER_AVOID_SLEEP_WHEN_SERIAL != 0);

void setup()
{
    board::disableRadio();
    board::setCPUFrequency();

    Serial.begin(kSerialBaudRate);
    delay(1000); // Give USB-serial a moment to come up.

    Wire.begin(kI2cSdaPin, kI2cSclPin);
    Wire.setClock(kI2cClockHz);
    delay(10);

    // Initialize peripherals.
    neoled::off();
    led::init();
    display::init();
    sensor::init();

    board::printInfo();
}

void loop()
{
    led::loop();
    sensor::loop();

    uint16_t co2 = sensor::getCO2();
    uint16_t temp = sensor::getTemperature();
    uint16_t hum = sensor::getHumidity();

    led::setErrorState(co2 > kMaxCO2ppm);

    display::setValues(temp, hum, co2);
    display::loop();

    if (kEnableLightSleepBetweenLoops && (!kAvoidSleepWhenSerialConnected || !Serial))
    {
        board::lightSleepMs(kLoopIntervalMs);
    }
    else
    {
        delay(kLoopIntervalMs);
    }
}