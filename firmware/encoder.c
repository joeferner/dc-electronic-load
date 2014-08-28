#include "encoder.h"
#include "platform_config.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include "debug.h"

#define ENCODER_P_INIT 0xff
uint8_t encoder_last_p;
int encoder_success_count;

void encoder_setup() {
  debug_write_line("?encoder_setup");

  encoder_last_p = ENCODER_P_INIT;

  GPIO_InitTypeDef gpioInitStructure;
  EXTI_InitTypeDef extiInitStructure;
  NVIC_InitTypeDef nvicInitStructure;

  RCC_APB2PeriphClockCmd(ENCODER_CH_A_RCC | ENCODER_CH_B_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);
  gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  gpioInitStructure.GPIO_Pin = ENCODER_CH_A_PIN;
  GPIO_Init(ENCODER_CH_A_PORT, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = ENCODER_CH_B_PIN;
  GPIO_Init(ENCODER_CH_B_PORT, &gpioInitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  GPIO_EXTILineConfig(ENCODER_CH_A_EXTI_PORT, ENCODER_CH_A_EXTI_PIN);
  GPIO_EXTILineConfig(ENCODER_CH_B_EXTI_PORT, ENCODER_CH_B_EXTI_PIN);

  EXTI_StructInit(&extiInitStructure);
  extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  extiInitStructure.EXTI_LineCmd = ENABLE;

  extiInitStructure.EXTI_Line = ENCODER_CH_A_EXTI;
  EXTI_Init(&extiInitStructure);

  extiInitStructure.EXTI_Line = ENCODER_CH_B_EXTI;
  EXTI_Init(&extiInitStructure);

  nvicInitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  nvicInitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInitStructure);

  EXTI_GenerateSWInterrupt(ENCODER_CH_A_EXTI);
}

// 0x00, 0x10, 0x11, 0x01
void encoder_exti_irq() {
  uint8_t a = GPIO_ReadInputDataBit(ENCODER_CH_A_PORT, ENCODER_CH_A_PIN);
  uint8_t b = GPIO_ReadInputDataBit(ENCODER_CH_B_PORT, ENCODER_CH_B_PIN);
  uint8_t p = (a ? 0x00 : 0x10) | (b ? 0x00 : 0x01);

  if (encoder_last_p == ENCODER_P_INIT) {
    if (p == 0x00) {
      encoder_last_p = p;
      encoder_success_count = 0;
    }
  } else if (p != encoder_last_p) {
    if (p == 0x00) {
      if (encoder_success_count == 3 && encoder_last_p == 0x01) {
        encoder_irq(ENCODER_DIR_CW);
        encoder_last_p = p;
        encoder_success_count = 0;
      } else if (encoder_success_count == -3 && encoder_last_p == 0x10) {
        encoder_irq(ENCODER_DIR_CCW);
        encoder_last_p = p;
        encoder_success_count = 0;
      } else {
        encoder_last_p = ENCODER_P_INIT;
      }
    } else if (p == 0x10) {
      if (encoder_success_count == 0 && encoder_last_p == 0x00) {
        encoder_success_count++;
        encoder_last_p = p;
      } else if (encoder_success_count == -2 && encoder_last_p == 0x11) {
        encoder_success_count--;
        encoder_last_p = p;
      } else {
        encoder_last_p = ENCODER_P_INIT;
      }
    } else if (p == 0x11) {
      if (encoder_success_count == 1 && encoder_last_p == 0x10) {
        encoder_success_count++;
        encoder_last_p = p;
      } else if (encoder_success_count == -1 && encoder_last_p == 0x01) {
        encoder_success_count--;
        encoder_last_p = p;
      } else {
        encoder_last_p = ENCODER_P_INIT;
      }
    } else if (p == 0x01) {
      if (encoder_success_count == 2 && encoder_last_p == 0x11) {
        encoder_success_count++;
        encoder_last_p = p;
      } else if (encoder_success_count == 0 && encoder_last_p == 0x00) {
        encoder_success_count--;
        encoder_last_p = p;
      } else {
        encoder_last_p = ENCODER_P_INIT;
      }
    }
  }
}
