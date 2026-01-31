# Senzor1 (ESP32-S3)

An ESP32-S3 (DevKitC-1) project that measures air quality (SCD41) and renders values on an OLED.
The goal is a simple low-power device that periodically reads the sensor and displays the values.

## Components

- **MCU**: ESP32-S3 DevKitC-1 (N16R8)
- **CO₂ / temperature / humidity sensor**: Sensirion **SCD41** (via I²C)
  - library: `sparkfun/SparkFun SCD4x Arduino Library`
- **OLED display**: SSD1306 128×64 (via I²C)
  - library: `U8g2`
- **NeoPixel**: 1× WS2812/NeoPixel
  - library: `Adafruit NeoPixel`
- **Status LEDs + beeper**: green (heartbeat), red (error) + beeper (beeps on error)

## Application logic

- `setup()`:
  - disables Wi‑Fi/Bluetooth (`board::disableRadio()`)
  - sets CPU frequency (`board::setCPUFrequency()`)
  - initializes `Serial` and I²C (`Wire.begin(SDA,SCL)`)
  - initializes peripherals: NeoPixel (`neoled::off()`), LEDs/beeper (`led::init()`), OLED (`display::init()`), sensor (`sensor::init()`)
  - prints diagnostics (`board::printInfo()`)

- `loop()`:
  - handles LEDs/beeper (`led::loop()`) – non-blocking heartbeat, periodic beeps in error state
  - polls the SCD41 (`sensor::loop()`) and keeps the latest readings cached
  - sets the error state based on the CO₂ threshold (`kMaxCO2ppm`)
  - updates the OLED (`display::setValues(...)` + `display::loop()`); the display redraws only when values change
  - optionally enters **light sleep** between iterations (`board::lightSleepMs(...)`) to reduce power consumption

## Pin configuration (build-time)

There are no hardcoded GPIO numbers in the C++ sources. All pins are configured via `build_flags` in [platformio.ini](platformio.ini) and the header [include/pins.h](include/pins.h).

Required defines:

- `I2C_SDA_PIN` – I²C SDA
- `I2C_SCL_PIN` – I²C SCL
- `LED_GREEN_PIN` – green LED
- `LED_RED_PIN` – red LED
- `NEOLED_PIN` – NeoPixel
- `BEEPER_PIN` – beeper

Example (current values in `platformio.ini`):

- `-DI2C_SDA_PIN=9`
- `-DI2C_SCL_PIN=8`
- `-DLED_GREEN_PIN=6`
- `-DLED_RED_PIN=7`
- `-DNEOLED_PIN=48`
- `-DBEEPER_PIN=4`

## Power tuning

The main loop behavior is controlled by macros (can be defined in `build_flags`):

- `POWER_LOOP_INTERVAL_MS` – main loop interval
- `POWER_ENABLE_LIGHT_SLEEP` – `1/0` enables light sleep between iterations
- `POWER_AVOID_SLEEP_WHEN_SERIAL` – `1/0` avoids sleeping while Serial is active

Note: light sleep has the biggest effect when the main loop interval is longer (e.g., 5–30 s) and Serial logging is limited.

## Wiring (typical)

- I²C:
  - SDA → GPIO per `I2C_SDA_PIN`
  - SCL → GPIO per `I2C_SCL_PIN`
  - recommended: external pull-up resistors ~4.7 kΩ to 3.3 V (if not present on modules)
- Power:
  - ESP32-S3 GPIO are **3.3 V** – I²C devices must be compatible
  - common ground (GND) between the board and peripherals

## Build & Upload (PlatformIO)

- Build: `platformio run`
- Upload: `platformio run --target upload`
- Serial monitor: `platformio device monitor`

## Project structure

- `src/main.cpp` – main application
- `src/sensor.cpp` + `include/sensor.h` – SCD41 (CO₂/T/RH)
- `src/display.cpp` + `include/display.h` – OLED rendering
- `src/led.cpp` + `include/led.h` – heartbeat/error LED + beeper
- `src/board.cpp` + `include/board.h` – board utilities (time, radio disable, light sleep)
- `include/pins.h` – build-time pin mapping

## Style notes

- 4-space indentation, consistent indentation inside `namespace` blocks.
- `.editorconfig` enforces basic editor rules.
- `.clang-format` includes `NamespaceIndentation: All` (if you use clang-format locally).
