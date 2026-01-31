#include "board.h"

#include <Arduino.h>
#include <U8g2lib.h>

namespace
{
    // SSD1306 I2C address is typically 0x3C (sometimes 0x3D depending on board).
    constexpr uint8_t kOledI2cAddress7Bit = 0x3C;

    // Anti burn-in settings.
    // Contrast: 0..255, lower reduces OLED power usage.
    constexpr uint8_t kOledContrast = 0;
    constexpr int64_t kShiftPeriodMs = 5000;   // shift every 5 seconds
    constexpr int kMaxShiftPx = 7;             // max horizontal+vertical shift in pixels
    constexpr int64_t kBlankPeriodMs = 90000;  // blank every 90 seconds
    constexpr int64_t kBlankDurationMs = 1000; // blank duration 1 second

    constexpr int kDisplayWidth = 128;
    constexpr int kTopLineY = 24;
    constexpr int kCO2LineY = 60;
    constexpr int kCO2SubscriptY = 63;
    constexpr int kMarginPx = 1;

    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

    int64_t lastShiftAtMs = 0;

    int displayShiftPx = 0;

    int temperatureSensorValue = 0;
    int humiditySensorValue = 0;
    int co2SensorValue = 0;

    void maybeShift()
    {
        const int64_t nowMs = board::now();
        if (nowMs - lastShiftAtMs >= kShiftPeriodMs)
        {
            lastShiftAtMs = nowMs;
            static uint8_t phase = 0;
            phase = static_cast<uint8_t>((phase + 1) % (kMaxShiftPx + 1));
            displayShiftPx = phase;
        }
    }

} // namespace

namespace display
{
    void init()
    {
        // U8g2 expects the 8-bit (shifted) address.
        u8g2.setI2CAddress(static_cast<uint8_t>(kOledI2cAddress7Bit << 1));
        u8g2.begin();
        u8g2.enableUTF8Print();
        u8g2.setContrast(kOledContrast);
        u8g2.setPowerSave(0);
    }

    void setValues(int temperature, int humidity, int co2)
    {
        temperatureSensorValue = temperature;
        humiditySensorValue = humidity;
        co2SensorValue = co2;
    }

    void loop()
    {
        // Store previous values to detect changes
        static int lastT = -1;
        static int lastH = -1;
        static int lastC = -1;
        static int lastShift = -1;

        maybeShift();

        const bool changed = (temperatureSensorValue != lastT) || (humiditySensorValue != lastH) ||
                             (co2SensorValue != lastC) || (displayShiftPx != lastShift);

        if (!changed)
        {
            return;
        }

        // Update change-tracking state.
        lastT = temperatureSensorValue;
        lastH = humiditySensorValue;
        lastC = co2SensorValue;
        lastShift = displayShiftPx;

        // Redraw display.
        u8g2.clearBuffer();

        // Upper half: large temperature (left) and humidity (right), without labels
        char bufT[32], bufH[32];
        snprintf(bufT, sizeof(bufT), "T%d°C", temperatureSensorValue);
        snprintf(bufH, sizeof(bufH), "H%d%%", humiditySensorValue);

        u8g2.setFont(u8g2_font_helvB10_tf);

        // Temperature
        u8g2.drawUTF8(kMarginPx + displayShiftPx, kMarginPx + displayShiftPx + kTopLineY, bufT);

        // Humidity
        const int widthH = u8g2.getStrWidth(bufH);
        u8g2.drawStr((kDisplayWidth - kMarginPx) - widthH - displayShiftPx,
                     kMarginPx + displayShiftPx + kTopLineY, bufH);

        // Lower half: CO2 with label
        char bufC[32];
        snprintf(bufC, sizeof(bufC), "CO   %d ppm", co2SensorValue);

        const int widthC = u8g2.getStrWidth(bufC);
        u8g2.drawStr((kDisplayWidth - widthC) / 2, kCO2LineY - kMarginPx - displayShiftPx, bufC);

        const int widthCO2 = u8g2.getStrWidth("CO");
        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr((kDisplayWidth - widthC) / 2 + widthCO2,
                     kCO2SubscriptY - kMarginPx - displayShiftPx, "2");

        u8g2.sendBuffer();
    }
} // namespace display