
#include "fan.h"
#include "platform_config.h"
#include "debug.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

void fan_setup() {
  GPIO_InitTypeDef gpioInitStructure;
  TIM_TimeBaseInitTypeDef timeBaseInitStructure;

  debug_write_line("?fan_setup");

  RCC_APB1PeriphClockCmd(FAN_PWM_RCC1, ENABLE);
  RCC_APB2PeriphClockCmd(FAN_PWM_RCC2, ENABLE);
#ifdef FAN_PWM_TIMER_REMAP
  GPIO_PinRemapConfig(FAN_PWM_TIMER_REMAP, ENABLE);
#endif

  GPIO_StructInit(&gpioInitStructure);
  gpioInitStructure.GPIO_Pin = FAN_PWM_PIN;
#ifdef FAN_PWM_TIMER_REMAP
  gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
#else
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#endif
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FAN_PWM_PORT, &gpioInitStructure);

  TIM_TimeBaseStructInit(&timeBaseInitStructure);
  timeBaseInitStructure.TIM_Period = 1000;
  timeBaseInitStructure.TIM_Prescaler = 72;
  timeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(FAN_PWM_TIMER, &timeBaseInitStructure);

  fan_set(100);
  FAN_PWM_TIMER_CH_PRELOAD_CONFIG(FAN_PWM_TIMER, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(FAN_PWM_TIMER, ENABLE);
  TIM_Cmd(FAN_PWM_TIMER, ENABLE);
}

void fan_set(uint8_t percent) {
  TIM_OCInitTypeDef ocInitStructure;

  uint32_t pulse = ((uint32_t)percent) * 10;

  TIM_OCStructInit(&ocInitStructure);
  ocInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  ocInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  ocInitStructure.TIM_Pulse = pulse;
  ocInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  FAN_PWM_TIMER_CH_INIT(FAN_PWM_TIMER, &ocInitStructure);
}
