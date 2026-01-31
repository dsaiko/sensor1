#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

namespace board
{
    // Milliseconds since boot (monotonic).
    int64_t now();

    // Disable Wi-Fi and Bluetooth to reduce power consumption.
    void disableRadio();

    // Configure CPU frequency (trade performance vs. power usage).
    void setCPUFrequency();

    // Enter light sleep for the given duration (ms) and wake on timer.
    //
    // Light sleep can significantly reduce average current consumption when the
    // application is otherwise just waiting (e.g., between sensor updates).
    void lightSleepMs(uint32_t sleepMs);
} // namespace board

#endif // BOARD_H
