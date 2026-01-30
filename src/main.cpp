#include <Arduino.h>
#include <WiFi.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"

#include "info.h"
#include "neoled.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  board::printInfo();
  neoled::Off();
}

void loop()
{
}