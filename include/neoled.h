#ifndef NEOLED_H
#define NEOLED_H

// NeoPixel LED helpers.
namespace neoled
{
    // Initialize the NeoPixel and turn it off (clear + show).
    // Pin is configured via build flag `NEOLED_PIN`.
    void off();
} // namespace neoled

#endif // NEOLED_H
