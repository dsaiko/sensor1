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
} // namespace board

#endif // BOARD_H
