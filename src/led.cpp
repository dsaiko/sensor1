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
        pinMode(pins::kLedGreen, OUTPUT);
        digitalWrite(pins::kLedGreen, LOW);

        pinMode(pins::kLedRed, OUTPUT);
        digitalWrite(pins::kLedRed, LOW);

        // Configure beeper GPIO.
        pinMode(pins::kBeeper, OUTPUT);
        digitalWrite(pins::kBeeper, LOW);
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
                tone(pins::kBeeper, kBeepFrequencyHz, static_cast<uint32_t>(kBeepDurationMs));
            }

            return;
        }

        // Turn the heartbeat LED off when the flash window ends.
        if (greenOnUntilMs != 0 && nowMs >= greenOnUntilMs)
        {
            digitalWrite(pins::kLedGreen, LOW);
            greenOnUntilMs = 0;
        }

        // Start new flash after period
        if (nowMs - lastHeartbeatAtMs >= kHeartbeatPeriodMs)
        {
            digitalWrite(pins::kLedGreen, HIGH);
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
            digitalWrite(pins::kLedRed, HIGH);
            digitalWrite(pins::kLedGreen, LOW);

            greenOnUntilMs = 0;
            lastHeartbeatAtMs = 0;
            lastBeepAtMs = 0;
        }
        else
        {
            // Clear red LED (heartbeat resumes on the next loop() call).
            digitalWrite(pins::kLedRed, LOW);
            digitalWrite(pins::kLedGreen, LOW);

            greenOnUntilMs = 0;
            lastHeartbeatAtMs = 0;
            lastBeepAtMs = 0;
        }
    }
} // namespace led
