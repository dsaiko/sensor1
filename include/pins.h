#ifndef PINS_H
#define PINS_H

// Centralized GPIO/pin configuration.
//
// All pins must be provided via build flags in `platformio.ini` (or another
// build system). This avoids hardcoded GPIO numbers in the source.
//
// Required build defines:
// - I2C_SDA_PIN
// - I2C_SCL_PIN
// - LED_GREEN_PIN
// - LED_RED_PIN
// - NEOLED_PIN
// - BEEPER_PIN

#ifndef I2C_SDA_PIN
#error "Missing build flag: -DI2C_SDA_PIN=<gpio>"
#endif

#ifndef I2C_SCL_PIN
#error "Missing build flag: -DI2C_SCL_PIN=<gpio>"
#endif

#ifndef LED_GREEN_PIN
#error "Missing build flag: -DLED_GREEN_PIN=<gpio>"
#endif

#ifndef LED_RED_PIN
#error "Missing build flag: -DLED_RED_PIN=<gpio>"
#endif

#ifndef NEOLED_PIN
#error "Missing build flag: -DNEOLED_PIN=<gpio>"
#endif

#ifndef BEEPER_PIN
#error "Missing build flag: -DBEEPER_PIN=<gpio>"
#endif

namespace pins
{
    constexpr int kI2cSda = I2C_SDA_PIN;
    constexpr int kI2cScl = I2C_SCL_PIN;

    constexpr int kLedGreen = LED_GREEN_PIN;
    constexpr int kLedRed = LED_RED_PIN;

    constexpr int kNeoPixel = NEOLED_PIN;
    constexpr int kBeeper = BEEPER_PIN;
} // namespace pins

#endif // PINS_H
