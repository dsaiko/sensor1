#include <Arduino.h>
#include <esp_wifi.h>
#include <esp_bt.h>

#include "board.h"

namespace
{
    constexpr uint32_t kCPUFrequency = 80;
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
        setCpuFrequencyMhz(kCPUFrequency);
    }

} // namespace board