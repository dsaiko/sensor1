#ifndef DISPLAY_H
#define DISPLAY_H

// OLED display rendering (SSD1306 128x64 over I2C via U8g2).
namespace display
{
// Initialize the display. Requires I2C (`Wire`) to be initialized first.
void init();

// Draw the current values to the screen. Call frequently from loop().
void draw();

// Update sensor values that will be displayed.
void setValues(int temperature, int humidity, int co2);
} // namespace display

#endif // DISPLAY_H
