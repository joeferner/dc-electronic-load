#include <stm32f10x_gpio.h>
#include <stdio.h>
#include "mock_stm32.h"

#define PIN_STATE_GPIOA  0
#define PIN_STATE_GPIOB  1
#define PIN_STATE_GPIOC  2

uint8_t gpioPinState[3][15];

void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct) {
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct) {
}

void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource) {
}

void MOCK_GPIO_SetPinState(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t state) {
  if (GPIOx == GPIOB) {
    if (gpioPinState[PIN_STATE_GPIOB][GPIO_Pin] != state) {
      gpioPinState[PIN_STATE_GPIOB][GPIO_Pin] = state;
      EXTI9_5_IRQHandler();
    }
  } else {
    printf("TODO: MOCK_GPIO_SetPinState: not implemented.");
  }
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
  if (GPIOx == GPIOB) {
    return gpioPinState[PIN_STATE_GPIOB][GPIO_Pin];
  } else {
    printf("TODO: GPIO_ReadInputDataBit: not implemented.");
  }
}
