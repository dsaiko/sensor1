#include "board.h"

#include <Arduino.h>
#include <esp_bt.h>
#include <esp_sleep.h>
#include <esp_timer.h>
#include <esp_wifi.h>

namespace
{
    constexpr uint32_t kCpuFrequencyMhz = 80;
} // namespace

namespace board
{
    int64_t now()
    {
        return (int64_t)(esp_timer_get_time() / 1000);
    }

    void disableRadio()
    {
        // Disable WiFi
        esp_wifi_stop();
        esp_wifi_deinit();

        // Disable Bluetooth
        esp_bt_controller_disable();
        esp_bt_controller_deinit();
    }

    void setCPUFrequency()
    {
        setCpuFrequencyMhz(kCpuFrequencyMhz);
    }

    void lightSleepMs(uint32_t sleepMs)
    {
        if (sleepMs == 0)
        {
            return;
        }

        // Timer wakeup expects microseconds.
        esp_sleep_enable_timer_wakeup(static_cast<uint64_t>(sleepMs) * 1000ULL);
        esp_light_sleep_start();
    }

} // namespace board