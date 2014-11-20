
#include "platform_config.h"
#include "buttons.h"
#include "debug.h"
#include "time.h"
#include <stm32f10x_gpio.h>

uint8_t buttonsLastReading;
uint32_t buttonsLastChangeTime;

void buttons_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?buttons_setup");

  buttonsLastReading = 0;
  buttonsLastChangeTime = 0;

  RCC_APB2PeriphClockCmd(BUTTONS_A_RCC | BUTTONS_B_RCC | BUTTONS_C_RCC | BUTTONS_D_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);
  gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  gpioInitStructure.GPIO_Pin = BUTTONS_A_PIN;
  GPIO_Init(BUTTONS_A_PORT, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = BUTTONS_B_PIN;
  GPIO_Init(BUTTONS_B_PORT, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = BUTTONS_C_PIN;
  GPIO_Init(BUTTONS_C_PORT, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = BUTTONS_D_PIN;
  GPIO_Init(BUTTONS_D_PORT, &gpioInitStructure);
}

#define IS_RISING(state, buttonMask) (state && ((buttonsLastReading & buttonMask) == 0x00))

void buttons_loop() {
  uint8_t a = !GPIO_ReadInputDataBit(BUTTONS_A_PORT, BUTTONS_A_PIN);
  uint8_t b = !GPIO_ReadInputDataBit(BUTTONS_B_PORT, BUTTONS_B_PIN);
  uint8_t c = !GPIO_ReadInputDataBit(BUTTONS_C_PORT, BUTTONS_C_PIN);
  uint8_t d = !GPIO_ReadInputDataBit(BUTTONS_D_PORT, BUTTONS_D_PIN);
  uint8_t v = 0;
  uint32_t time = time_ms();

  if (IS_RISING(a, BUTTON_A)) {
    v |= BUTTON_A;
  }
  if (IS_RISING(b, BUTTON_B)) {
    v |= BUTTON_B;
  }
  if (IS_RISING(c, BUTTON_C)) {
    v |= BUTTON_C;
  }
  if (IS_RISING(d, BUTTON_D)) {
    v |= BUTTON_D;
  }

  if (v != buttonsLastReading) {
    if ((time - buttonsLastChangeTime) < 100) {
      return;
    }

    buttonsLastChangeTime = time;
    buttonsLastReading = v;
    buttons_irq(v);
  }
}
