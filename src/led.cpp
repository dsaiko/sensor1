#include <Arduino.h>

#include "led.h"
#include "board.h"

namespace
{

} // namespace

namespace led
{

    static const int CONTROL_LED_PERIOD_MS = 3000;
    static const int CONTROL_LED_FLASH_MS = 50;

    static const int LED_GREEN_PIN = 6;
    static const int LED_RED_PIN = 7;

    static bool errorState = false;

    void init()
    {
        pinMode(LED_GREEN_PIN, OUTPUT);
        digitalWrite(LED_GREEN_PIN, LOW);

        pinMode(LED_RED_PIN, OUTPUT);
        digitalWrite(LED_RED_PIN, LOW);
    }

    void operate()
    {
        static int64_t controlLedOffAt = 0;
        static int64_t lastControlLedBlink = 0;

        int64_t now = board::now();

        if (errorState)
        {
            // Solid red LED in error state
            digitalWrite(LED_RED_PIN, HIGH);
            digitalWrite(LED_GREEN_PIN, LOW);
            return;
        }

        digitalWrite(LED_RED_PIN, LOW);

        // Turn off LED after flash duration
        if (controlLedOffAt != 0 && now >= controlLedOffAt)
        {
            digitalWrite(LED_GREEN_PIN, LOW);
            controlLedOffAt = 0;
        }

        // Start new flash after period
        if (now - lastControlLedBlink >= CONTROL_LED_PERIOD_MS)
        {
            digitalWrite(LED_GREEN_PIN, HIGH);

            controlLedOffAt = now + CONTROL_LED_FLASH_MS;
            lastControlLedBlink = now;
        }
    }

    void setErrorState(bool isError)
    {
        errorState = isError;
    }
}
