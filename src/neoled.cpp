#include "neoled.h"

#include <Adafruit_NeoPixel.h>

// Default pin can be overridden at build time.
#ifndef NEOLED_PIN
#define NEOLED_PIN 48
#endif

namespace
{
    constexpr uint8_t kNeoLedPin = NEOLED_PIN;
    Adafruit_NeoPixel neoPixel(1, kNeoLedPin, NEO_GRB + NEO_KHZ800);
} // namespace

namespace neoled
{
    void off()
    {
        // Ensure the LED is initialized before turning it off.
        neoPixel.begin();
        neoPixel.setBrightness(0);
        neoPixel.clear();
        neoPixel.show();
    }
} // namespace neoled
