#include <stm32f10x_exti.h>
#include <stdio.h>

#define EXTI_LINENONE    ((uint32_t)0x00000)  /* No interrupt selected */

extern void EXTI9_5_IRQHandler();

void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct) {
  EXTI_InitStruct->EXTI_Line = EXTI_LINENONE;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct) {
}

void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line) {
  switch (EXTI_Line) {
    case EXTI_Line5:
    case EXTI_Line6:
    case EXTI_Line7:
    case EXTI_Line8:
    case EXTI_Line9:
      EXTI9_5_IRQHandler();
      break;
    default:
      printf("TODO: EXTI_GenerateSWInterrupt: not implemented.");
      break;
  }
}
