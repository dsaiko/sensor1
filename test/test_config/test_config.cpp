#include "pins.h"

#include <Arduino.h>
#include <unity.h>

// If these are not provided via build flags, compilation fails already.
// These runtime checks make failures more visible in CI logs.

void setUp() {}
void tearDown() {}

static void test_i2c_pins_are_distinct()
{
    TEST_ASSERT_NOT_EQUAL(pins::kI2cSda, pins::kI2cScl);
}

static void test_status_led_pins_are_distinct()
{
    TEST_ASSERT_NOT_EQUAL(pins::kLedGreen, pins::kLedRed);
}

static void test_pins_are_not_negative()
{
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kI2cSda);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kI2cScl);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kLedGreen);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kLedRed);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kNeoPixel);
    TEST_ASSERT_GREATER_OR_EQUAL(0, pins::kBeeper);
}

static void test_power_macros_are_sane()
{
#ifdef POWER_LOOP_INTERVAL_MS
    TEST_ASSERT_GREATER_THAN(0, POWER_LOOP_INTERVAL_MS);
#endif

#ifdef POWER_ENABLE_LIGHT_SLEEP
    TEST_ASSERT_TRUE(POWER_ENABLE_LIGHT_SLEEP == 0 || POWER_ENABLE_LIGHT_SLEEP == 1);
#endif

#ifdef POWER_AVOID_SLEEP_WHEN_SERIAL
    TEST_ASSERT_TRUE(POWER_AVOID_SLEEP_WHEN_SERIAL == 0 || POWER_AVOID_SLEEP_WHEN_SERIAL == 1);
#endif
}

void setup()
{
    delay(2000); // Give time to open the serial monitor.

    UNITY_BEGIN();
    RUN_TEST(test_i2c_pins_are_distinct);
    RUN_TEST(test_status_led_pins_are_distinct);
    RUN_TEST(test_pins_are_not_negative);
    RUN_TEST(test_power_macros_are_sane);
    UNITY_END();
}

void loop() {}
