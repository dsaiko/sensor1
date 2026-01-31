#include "led.h"

#include "board.h"

#include <Arduino.h>

namespace
{
    // Heartbeat timing: brief green flash every period when healthy.
    constexpr int64_t kControlLedPeriodMs = 3000;
    constexpr int64_t kControlLedFlashMs = 50;

    // GPIO pins for the onboard status LEDs.
    constexpr uint8_t kLedGreenPin = 6;
    constexpr uint8_t kLedRedPin = 7;

    // Timing state for the heartbeat LED.
    int64_t controlLedOnUntil = 0;
    int64_t lastControlLedBlink = 0;

    // Tracks whether the system is in error state.
    bool errorState = false;
} // namespace

namespace led
{
    void init()
    {
        // Configure LED GPIOs in a known OFF state.
        pinMode(kLedGreenPin, OUTPUT);
        digitalWrite(kLedGreenPin, LOW);

        pinMode(kLedRedPin, OUTPUT);
        digitalWrite(kLedRedPin, LOW);
    }

    void run()
    {
        // board::now() is expected to return milliseconds since boot.
        const int64_t now = board::now();

        // Turn off LED after flash duration
        if (controlLedOnUntil != 0 && now >= controlLedOnUntil)
        {
            digitalWrite(kLedGreenPin, LOW);
            controlLedOnUntil = 0;
        }

        // Start new flash after period
        if (now - lastControlLedBlink >= kControlLedPeriodMs)
        {
            digitalWrite(kLedGreenPin, HIGH);

            controlLedOnUntil = now + kControlLedFlashMs;
            lastControlLedBlink = now;
        }
    }

    void setErrorState(bool isError)
    {
        // Do nothing if state is unchanged.
        if (errorState == isError)
        {
            return;
        }

        errorState = isError;
        if (errorState)
        {
            // Solid red LED.
            digitalWrite(kLedRedPin, HIGH);
        }
        else
        {
            // Clear red LED (heartbeat resumes on the next run() call).
            digitalWrite(kLedRedPin, LOW);
        }
    }
} // namespace led
