#include "neoled.h"

#include <Adafruit_NeoPixel.h>

// Default pin can be overridden at build time.
#ifndef NEOLED_PIN
#define NEOLED_PIN 48
#endif

namespace
{
    constexpr uint8_t kNeoLedPin = NEOLED_PIN;
    Adafruit_NeoPixel led(1, kNeoLedPin, NEO_GRB + NEO_KHZ800);
} // namespace

namespace neoled
{
    void Off()
    {
        // Ensure the LED is initialized before turning it off.
        led.begin();
        led.setBrightness(0);
        led.clear();
        led.show();
    }
}
