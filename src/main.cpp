#include <Arduino.h>
#include <WiFi.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"

#include "info.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef LED_PIN
#define LED_PIN 48 // podle desky z Dratek.cz
#endif

void setup()
{
  Serial.begin(115200);
  delay(10000);

  printBoardInfo();
}

void loop()
{
}