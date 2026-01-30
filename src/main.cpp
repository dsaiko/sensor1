#include <Arduino.h>
#include <WiFi.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"

#include "info.h"
#include "neoled.h"
#include "led.h"
#include "board.h"

constexpr int64_t kSerialBaudRate = 115200;

void setup()
{
  board::disableRadio();
  board::setCPUFrequency();

  Serial.begin(kSerialBaudRate);
  led::init();

  delay(1000);

  board::printInfo();
  neoled::off();
}

void loop()
{
  led::operate();

  // demo error state toggle
  if (board::now() > 10000 && board::now() < 20000)
  {
    // Simulate error after 10 seconds
    led::setErrorState(true);
  }
  else
  {
    // Simulate error after 10 seconds
    led::setErrorState(false);
  }

  delay(100);
}