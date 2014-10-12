#include <stm32f10x_exti.h>
#include "platform_config.h"
#include "debug.h"

extern void time_SysTick_Handler();
extern void encoder_exti_irq();

static void HardFault_Handler(void) __attribute__((naked));
void prvGetRegistersFromStack(uint32_t* pulFaultStackAddress);

void NMI_Handler() {
}

static void HardFault_Handler() {
  __asm volatile
  (
    " tst lr, #4                                                \n"
    " ite eq                                                    \n"
    " mrseq r0, msp                                             \n"
    " mrsne r0, psp                                             \n"
    " ldr r1, [r0, #24]                                         \n"
    " ldr r2, handler2_address_const                            \n"
    " bx r2                                                     \n"
    " handler2_address_const: .word prvGetRegistersFromStack    \n"
  );
  while (1);
}

void MemManage_Handler() {
}

void BusFault_Handler() {
  debug_write_line("BusFault_Handler");
  while (1);
}

void UsageFault_Handler() {
  debug_write_line("UsageFault_Handler");
  while (1);
}

void SVC_Handler() {
}

void DebugMon_Handler() {
}

void PendSV_Handler() {
}

void SysTick_Handler() {
  time_SysTick_Handler();
}

#ifdef ENCODER_ENABLE
void EXTI0_IRQHandler() {
  uint8_t signal = 0;

  if (EXTI_GetITStatus(ENCODER_CH_A_EXTI) != RESET) {
    signal = 1;
    EXTI_ClearITPendingBit(ENCODER_CH_A_EXTI);
  }

  if (signal) {
    encoder_exti_irq();
  }
}

void EXTI1_IRQHandler() {
  uint8_t signal = 0;

  if (EXTI_GetITStatus(ENCODER_CH_B_EXTI) != RESET) {
    signal = 1;
    EXTI_ClearITPendingBit(ENCODER_CH_B_EXTI);
  }

  if (signal) {
    encoder_exti_irq();
  }
}

#endif

void prvGetRegistersFromStack(uint32_t* pulFaultStackAddress) {
  /* These are volatile to try and prevent the compiler/linker optimising them
  away as the variables never actually get used.  If the debugger won't show the
  values of the variables, make them global my moving their declaration outside
  of this function. */
  volatile uint32_t r0;
  volatile uint32_t r1;
  volatile uint32_t r2;
  volatile uint32_t r3;
  volatile uint32_t r12;
  volatile uint32_t lr; /* Link register. */
  volatile uint32_t pc; /* Program counter. */
  volatile uint32_t psr;/* Program status register. */

  r0 = pulFaultStackAddress[ 0 ];
  r1 = pulFaultStackAddress[ 1 ];
  r2 = pulFaultStackAddress[ 2 ];
  r3 = pulFaultStackAddress[ 3 ];

  r12 = pulFaultStackAddress[ 4 ];
  lr = pulFaultStackAddress[ 5 ];
  pc = pulFaultStackAddress[ 6 ];
  psr = pulFaultStackAddress[ 7 ];

  /* When the following line is hit, the variables contain the register values. */
  debug_write_line("!!!!Fault!!!!");
  for (;;);
}

