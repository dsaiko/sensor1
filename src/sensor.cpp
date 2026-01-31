#include "sensor.h"

#include "SparkFun_SCD4x_Arduino_Library.h"
#include "board.h"

#include <Arduino.h>

namespace
{
    // Minimum interval between measurement reads.
    // The SCD4x produces new samples at a slower cadence than a tight loop,
    // so we throttle read attempts to keep I2C traffic and logging reasonable.
    // In low-power periodic measurement mode, new samples are typically produced
    // roughly every ~30 seconds. Polling much faster wastes I2C traffic and CPU.
    constexpr int64_t kSensorPollPeriodMs = 30000;

#ifndef SENSOR_LOG_READINGS
#define SENSOR_LOG_READINGS 0
#endif

    SCD4x scd41;

    int64_t lastPollAtMs = 0;

    uint16_t co2ppm = 0;
    uint16_t temperature = 0;
    uint16_t humidity = 0;

} // namespace

namespace sensor
{
    void init()
    {
        // SparkFun API: begin(skipStopPeriodicMeasurements, autoCalibrate)
        if (!scd41.begin(false, true))
        {
            Serial.println("SCD41 init failed. Check wiring/power and I2C pullups.");
            // Halt in a visible way; caller can't continue without sensor.
            for (;;)
            {
                delay(1000);
            }
        }

        if (scd41.startLowPowerPeriodicMeasurement())
        {
            Serial.println("SCD41 low power mode enabled.");
        }
        else
        {
            Serial.println("SCD41 low power mode NOT enabled.");
        }
    }

    void loop()
    {
        const int64_t nowMs = board::now();
        if (nowMs - lastPollAtMs < kSensorPollPeriodMs)
            return;

        lastPollAtMs = nowMs;

        // readMeasurement() returns true only when a fresh sample is ready.
        if (scd41.readMeasurement())
        {
            co2ppm = scd41.getCO2();
            temperature = static_cast<uint16_t>(scd41.getTemperature());
            humidity = static_cast<uint16_t>(scd41.getHumidity());

#if SENSOR_LOG_READINGS
            Serial.print("CO2(ppm):");
            Serial.print(co2ppm);

            Serial.print("\tTemperature(C):");
            Serial.print(temperature);

            Serial.print("\tHumidity(%RH):");
            Serial.print(humidity);

            Serial.println();
#endif
        }
    }

    uint16_t getCO2()
    {
        return co2ppm;
    }

    uint16_t getTemperature()
    {
        return temperature;
    }

    uint16_t getHumidity()
    {
        return humidity;
    }

} // namespace sensor
