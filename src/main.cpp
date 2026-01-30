#include <Arduino.h>
#include <WiFi.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"

#include "info.h"
#include "neoled.h"
#include "led.h"
#include "board.h"

static const int LED_PIN = 6;

void setup()
{
  Serial.begin(115200);
  led::init();

  delay(1000);

  board::printInfo();
  neoled::off();
}

void loop()
{
  led::operate();

  // demo error state toggle
  if (board::now() > 10000)
  {
    // Simulate error after 30 seconds
    led::setErrorState(true);
  }
  if (board::now() > 20000)
  {
    // Simulate error after 30 seconds
    led::setErrorState(false);
  }

  delay(100);
}