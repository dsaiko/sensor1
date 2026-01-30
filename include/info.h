#ifndef INFO_H
#define INFO_H

#include <Arduino.h>

// Print a concise diagnostic report (chip + memory + build info) to Serial.
// Call from setup() after Serial.begin().
void printBoardInfo();

#endif // INFO_H
