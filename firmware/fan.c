
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
  TIM_TimeBaseInitTypeDef timeBaseInitStructure;
  TIM_OCInitTypeDef ocInit;

  debug_write_line("?fan_setup");

  RCC_APB2PeriphClockCmd(FAN_PWM_RCC2, ENABLE);

  GPIO_StructInit(&gpioInitStructure);
  gpioInitStructure.GPIO_Pin = FAN_PWM_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FAN_PWM_PORT, &gpioInitStructure);

  TIM_TimeBaseStructInit(&timeBaseInitStructure);
  timeBaseInitStructure.TIM_Period = 1000;
  timeBaseInitStructure.TIM_Prescaler = 720;
  timeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(FAN_PWM_TIMER, &timeBaseInitStructure);

  ocInit.TIM_OCMode = TIM_OCMode_PWM2;
  ocInit.TIM_OutputState = TIM_OutputState_Enable;
  ocInit.TIM_OutputNState = TIM_OutputNState_Enable;
  ocInit.TIM_Pulse = (5 * (timeBaseInitStructure.TIM_Period - 1)) / 10;
  ocInit.TIM_OCPolarity = TIM_OCPolarity_Low;
  ocInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
  ocInit.TIM_OCIdleState = TIM_OCIdleState_Set;
  ocInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM1, &ocInit);

  fan_set(100);
  FAN_PWM_TIMER_CH_PRELOAD_CONFIG(FAN_PWM_TIMER, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(FAN_PWM_TIMER, ENABLE);
  TIM_Cmd(FAN_PWM_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
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

  fanCurrentPercent = percent;
}

uint8_t fan_get() {
  return fanCurrentPercent;
}

#endif
