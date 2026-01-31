#include "led.h"

#include "board.h"
#include "pins.h"

#include <Arduino.h>

namespace
{
    // Heartbeat timing: brief green flash every period when healthy.
    constexpr int64_t kHeartbeatPeriodMs = 5000;
    constexpr int64_t kHeartbeatFlashMs = 50;

    // Beeper pattern while in error state.
    constexpr unsigned int kBeepFrequencyHz = 2500;
    constexpr int64_t kBeepDurationMs = 50;
    constexpr int64_t kBeepPeriodMs = 5000;

    // GPIO pins for the onboard status LEDs.
    constexpr int kLedGreenPin = pins::kLedGreen;
    constexpr int kLedRedPin = pins::kLedRed;
    constexpr int kBeeperPin = pins::kBeeper;

    // Timing state for the heartbeat LED (non-blocking).
    int64_t greenOnUntilMs = 0;
    int64_t lastHeartbeatAtMs = 0;

    // Tracks whether the system is in error state.
    bool errorState = false;

    int64_t lastBeepAtMs = 0;
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
        const int64_t nowMs = board::now();

        // In error state we keep the red LED on and suppress the heartbeat.
        if (errorState)
        {
            if (nowMs - lastBeepAtMs >= kBeepPeriodMs)
            {
                lastBeepAtMs = nowMs;

                // tone() is asynchronous on ESP32; it stops automatically after duration.
                tone(kBeeperPin, kBeepFrequencyHz, static_cast<uint32_t>(kBeepDurationMs));
            }

            return;
        }

        // Turn the heartbeat LED off when the flash window ends.
        if (greenOnUntilMs != 0 && nowMs >= greenOnUntilMs)
        {
            digitalWrite(kLedGreenPin, LOW);
            greenOnUntilMs = 0;
        }

        // Start new flash after period
        if (nowMs - lastHeartbeatAtMs >= kHeartbeatPeriodMs)
        {
            digitalWrite(kLedGreenPin, HIGH);
            greenOnUntilMs = nowMs + kHeartbeatFlashMs;
            lastHeartbeatAtMs = nowMs;
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

            greenOnUntilMs = 0;
            lastHeartbeatAtMs = 0;
            lastBeepAtMs = 0;
        }
        else
        {
            // Clear red LED (heartbeat resumes on the next loop() call).
            digitalWrite(kLedRedPin, LOW);
            digitalWrite(kLedGreenPin, LOW);

            greenOnUntilMs = 0;
            lastHeartbeatAtMs = 0;
            lastBeepAtMs = 0;
        }
    }
} // namespace led
