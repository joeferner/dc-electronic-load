#ifndef _MOCK_STM32_H_
#define _MOCK_STM32_H_

#include <stm32f10x_gpio.h>

void MOCK_GPIO_SetPinState(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t state);

#endif // _MOCK_STM32_H_
