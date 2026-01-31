#ifndef SENSOR_H
#define SENSOR_H

#include <cstdint>

// SparkFun SCD4x (SCD41) sensor interface.
namespace sensor
{
    // Initialize the sensor and start periodic measurements.
    // Requires I2C (`Wire`) to be initialized first.
    void init();

    // Poll the sensor and update cached readings when new data is available.
    void loop();

    // Latest CO2 concentration in ppm.
    uint16_t getCO2();

    // Latest temperature in degrees Celsius (integer, truncated).
    uint16_t getTemperature();

    // Latest relative humidity in %RH (integer, truncated).
    uint16_t getHumidity();
} // namespace sensor

#endif // SENSOR_H
