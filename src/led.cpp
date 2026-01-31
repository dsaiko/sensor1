#include "led.h"

#include "board.h"
#include "pins.h"

#include <Arduino.h>

namespace
{
    // Heartbeat timing: brief green flash every period when healthy.
    constexpr int kControlLedPeriodMs = 5000;
    constexpr int kControlLedFlashMs = 50;

    constexpr int beepFrequency = 2500; // Hz
    constexpr int beepDurationMs = 50;  // milliseconds
    constexpr int beepPeriodMs = 5000;  // milliseconds

    // GPIO pins for the onboard status LEDs.
    constexpr int kLedGreenPin = pins::kLedGreen;
    constexpr int kLedRedPin = pins::kLedRed;
    constexpr int kBeeperPin = pins::kBeeper;

    // Timing state for the heartbeat LED.
    int64_t lastControlLedBlink = 0;

    // Tracks whether the system is in error state.
    bool errorState = false;

    int64_t lastBeep = 0;
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

        pinMode(kBeeperPin, OUTPUT);
        digitalWrite(kBeeperPin, LOW);
    }

    void loop()
    {
        // board::now() is expected to return milliseconds since boot.
        const int64_t now = board::now();

        // In error state we keep the red LED on and suppress the heartbeat.
        if (errorState)
        {
            if (now - lastBeep >= beepPeriodMs)
            {
                lastBeep = now;
                tone(kBeeperPin, beepFrequency, beepDurationMs);
                delay(beepDurationMs);
                digitalWrite(kBeeperPin, LOW);
            }

            return;
        }

        // Start new flash after period
        if (now - lastControlLedBlink >= kControlLedPeriodMs)
        {
            digitalWrite(kLedGreenPin, HIGH);
            delay(kControlLedFlashMs);
            digitalWrite(kLedGreenPin, LOW);
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
            // Solid red LED; heartbeat off.
            digitalWrite(kLedRedPin, HIGH);
            digitalWrite(kLedGreenPin, LOW);
        }
        else
        {
            // Clear red LED (heartbeat resumes on the next loop() call).
            digitalWrite(kLedRedPin, LOW);
            digitalWrite(kLedGreenPin, LOW);
            lastControlLedBlink = 0;
        }
    }
} // namespace led
