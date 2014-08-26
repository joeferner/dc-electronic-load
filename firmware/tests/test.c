#include "../stringUtils.h"
#include "../platform_config.h"
#include "../encoder.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mock_stm32.h"

#define assertEquals(expected, found, message)  \
  if(expected != found) {                       \
    printf("FAIL: %s:%d %s (expected: %d, found: %d)\n", __FILE__, __LINE__, message, expected, found); \
    return;                                     \
  }

void testAddCommas(const char* src, const char* expected);
void testPadLeft(const char* src, const char* expected, int totalLen);
void testEncoder();
void mock_encoder_setPinState(uint8_t chA, uint8_t chB);

int encoderValue;

int main(int argc, const char* argv[]) {
  testAddCommas("0", "0");
  testAddCommas("1", "1");
  testAddCommas("10", "10");
  testAddCommas("123", "123");
  testAddCommas("1234", "1,234");
  testAddCommas("12345", "12,345");
  testAddCommas("123456", "123,456");
  testAddCommas("1234567", "1,234,567");

  testPadLeft("123", "123", 2);
  testPadLeft("123", "123", 3);
  testPadLeft("123", " 123", 4);

  testEncoder();

  return 0;
}

void mock_encoder_setPinState(uint8_t chA, uint8_t chB) {
  MOCK_GPIO_SetPinState(ENCODER_CH_A_PORT, ENCODER_CH_A_PIN, chA);
  MOCK_GPIO_SetPinState(ENCODER_CH_B_PORT, ENCODER_CH_B_PIN, chB);
}

void testEncoder() {
  encoderValue = 0;
  mock_encoder_setPinState(1, 1);
  encoder_setup();

  // Clockwise 1
  mock_encoder_setPinState(0, 1);
  assertEquals(0, encoderValue, "Encoder CW");
  mock_encoder_setPinState(0, 0);
  assertEquals(0, encoderValue, "Encoder CW");
  mock_encoder_setPinState(1, 0);
  assertEquals(0, encoderValue, "Encoder CW");
  mock_encoder_setPinState(1, 1);
  assertEquals(1, encoderValue, "Encoder CW");

  // Clockwise 2
  mock_encoder_setPinState(0, 1);
  assertEquals(1, encoderValue, "Encoder CW");
  mock_encoder_setPinState(0, 0);
  assertEquals(1, encoderValue, "Encoder CW");
  mock_encoder_setPinState(1, 0);
  assertEquals(1, encoderValue, "Encoder CW");
  mock_encoder_setPinState(1, 1);
  assertEquals(2, encoderValue, "Encoder CW");

  // Counter Clockwise 3
  mock_encoder_setPinState(1, 0);
  assertEquals(2, encoderValue, "Encoder CCW");
  mock_encoder_setPinState(0, 0);
  assertEquals(2, encoderValue, "Encoder CCW");
  mock_encoder_setPinState(0, 1);
  assertEquals(2, encoderValue, "Encoder CCW");
  mock_encoder_setPinState(1, 1);
  assertEquals(1, encoderValue, "Encoder CCW");
}

void encoder_irq(ENCODER_DIR dir) {
  if(dir == ENCODER_DIR_CW) {
    encoderValue++;
  } else if(dir == ENCODER_DIR_CCW) {
    encoderValue--;
  } else {
    printf("FAIL: Invalid encoder direction %d\n", dir);
  }
}

void testAddCommas(const char* src, const char* expected) {
  char temp[20];

  addCommas(src, temp);
  if(strcmp(temp, expected) == 0) {
    printf("OK: %s -> %s\n", src, temp);
  } else {
    printf("FAIL: %s -> %s\n", src, temp);
  }
}

void testPadLeft(const char* src, const char* expected, int totalLen) {
  char temp[20];

  padLeft(src, temp, totalLen);
  if(strcmp(temp, expected) == 0) {
    printf("OK: %s -> %s\n", src, temp);
  } else {
    printf("FAIL: %s -> %s\n", src, temp);
  }
}

void EXTI9_5_IRQHandler() {
  encoder_exti_irq();
}
