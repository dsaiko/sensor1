#include "neoled.h"

#include "pins.h"

#include <Adafruit_NeoPixel.h>

namespace
{
    constexpr int kNeoLedPin = pins::kNeoPixel;
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
