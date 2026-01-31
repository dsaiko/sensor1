#ifndef LED_H
#define LED_H

// Status LED control (green heartbeat + red error indicator).
namespace led
{
    // Initialize LED GPIOs. Must be called before other functions.
    void init();

    // Run the LED state machine. Call frequently from loop().
    void run();

    // Enable/disable the error state (solid red LED).
    void setErrorState(bool isError);
} // namespace led

#endif // LED_H
