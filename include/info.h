#ifndef INFO_H
#define INFO_H

// Board/system diagnostics helpers.
namespace board
{
    // Print a concise diagnostic report (chip + memory + build info) to Serial.
    // Call from setup() after Serial.begin().
    void printInfo();
}

#endif // INFO_H
