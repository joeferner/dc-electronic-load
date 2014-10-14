
#include "fan.h"
#include "platform_config.h"
#include "debug.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

#ifdef FAN_ENABLE

uint8_t fanCurrentPercent;

void fan_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?fan_setup");

  RCC_APB2PeriphClockCmd(FAN_RCC2, ENABLE);

  GPIO_StructInit(&gpioInitStructure);
  gpioInitStructure.GPIO_Pin = FAN_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FAN_PORT, &gpioInitStructure);

  fan_set(100);
}

void fan_set(uint8_t percent) {
  if (percent > 50) {
    GPIO_SetBits(FAN_PORT, FAN_PIN);
  } else {
    GPIO_ResetBits(FAN_PORT, FAN_PIN);
  }

  fanCurrentPercent = percent;
}

uint8_t fan_get() {
  return fanCurrentPercent;
}

#endif
