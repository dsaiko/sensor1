#include "info.h"

#include "esp_chip_info.h"
#include "esp_system.h"

#include <Arduino.h>
#include <Wire.h>

namespace
{
    // Convert bytes to megabytes as a float for display purposes.
    inline float bytesToMegabytes(uint32_t bytes)
    {
        return bytes / 1024.0f / 1024.0f;
    }

    // Print chip and system-related details.
    void printChipInfo()
    {
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);

        Serial.println("\n=== CHIP / SYSTEM ===");
        Serial.printf("ESP.getChipModel(): %s\n", ESP.getChipModel());
        Serial.printf("ESP.getChipRevision(): %u\n", ESP.getChipRevision());
        Serial.printf("Cores: %d\n", chip_info.cores);
        Serial.printf("CPU freq: %u MHz\n", ESP.getCpuFreqMHz());
        Serial.printf("SDK: %s\n", ESP.getSdkVersion());
        Serial.printf("Sketch MD5: %s\n", ESP.getSketchMD5().c_str());

        const uint32_t flash_size = ESP.getFlashChipSize();
        Serial.printf("Flash size: %u bytes (%.2f MB)\n", flash_size, bytesToMegabytes(flash_size));
        Serial.printf("Flash speed: %u Hz\n", ESP.getFlashChipSpeed());
        Serial.printf("Flash mode: %u\n", ESP.getFlashChipMode()); // 0.. (core-specific)
        Serial.printf("PSRAM size: %u bytes\n", ESP.getPsramSize());
        Serial.printf("PSRAM free: %u bytes\n", ESP.getFreePsram());

        const uint64_t mac = ESP.getEfuseMac();
        Serial.printf("eFuse MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", static_cast<uint8_t>(mac >> 40),
                      static_cast<uint8_t>(mac >> 32), static_cast<uint8_t>(mac >> 24),
                      static_cast<uint8_t>(mac >> 16), static_cast<uint8_t>(mac >> 8),
                      static_cast<uint8_t>(mac));

        Serial.printf("Reset reason (CPU0): %d\n", esp_reset_reason());
    }

    // Print heap and sketch memory statistics.
    void printMemoryInfo()
    {
        Serial.println("\n=== MEMORY ===");
        Serial.printf("Heap size: %u\n", ESP.getHeapSize());
        Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
        Serial.printf("Min free heap (since boot): %u\n", ESP.getMinFreeHeap());
        Serial.printf("Max alloc heap: %u\n", ESP.getMaxAllocHeap());

        Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
        Serial.printf("Free sketch space: %u\n", ESP.getFreeSketchSpace());
    }

    // List I2C devices on the bus.
    void printI2CInfo()
    {
        Serial.println("\n=== I2C ===");

        uint8_t count = 0;

        for (uint8_t addr = 0x03; addr <= 0x77; addr++)
        {
            Wire.beginTransmission(addr);
            const uint8_t err = Wire.endTransmission();

            if (err == 0)
            {
                Serial.printf("I2C device found at 0x%02X\n", addr);
                count++;
            }
        }

        if (count == 0)
        {
            Serial.println("No I2C devices found.");
        }
        else
        {
            Serial.printf("I2C devices found: %u\n", count);
        }
    }
} // namespace

// Public diagnostics API.
namespace board
{
    void printInfo()
    {
        // Overall diagnostics header with build information.
        Serial.println("\n\n========== ESP32 DIAGNOSTICS ==========");
        Serial.printf("Build: %s %s\n", __DATE__, __TIME__);

        printChipInfo();
        printMemoryInfo();
        printI2CInfo();
    }
} // namespace board