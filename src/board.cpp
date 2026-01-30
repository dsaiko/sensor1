#include <Arduino.h>
#include "board.h"

namespace board
{
    int64_t now()
    {
        return (int64_t)(esp_timer_get_time() / 1000);
    }
} // namespace board