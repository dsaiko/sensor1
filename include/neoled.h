#ifndef NEOLED_H
#define NEOLED_H

// NeoPixel LED helpers.
namespace neoled
{
    // Initialize the NeoPixel and turn it off (clear + show).
    // Uses the default pin defined by NEOLED_PIN unless overridden.
    void off();
} // namespace neoled

#endif // NEOLED_H
