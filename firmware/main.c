#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include <misc.h>
#include <string.h>
#include <stdlib.h>
#include "platform_config.h"
#include "debug.h"
#include "delay.h"
#include "dcElectronicLoad.h"

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)

void setup();
void loop();

PROCINIT(
  &etimer_process
  , &debug_process
#ifdef DISP6800_ENABLE
  , &gfx_update_process
#endif
#ifdef ADC_ENABLE
  , &adc_volts_process
  , &adc_current_process
#endif
#ifdef NETWORK_ENABLE
  , &tcpip_process
  , &dhcp_process
  , &httpd_process
  , &telnet_process
  , &resolv_process
#endif
);

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

void setup() {
  // Configure the NVIC Preemption Priority Bits
  // 2 bit for pre-emption priority, 2 bits for subpriority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  dc_electronic_load_setup();
}

void loop() {
  dc_electronic_load_loop();
}

void assert_failed(uint8_t* file, uint32_t line) {
  debug_write("-assert_failed: file ");
  debug_write((const char*) file);
  debug_write(" on line ");
  debug_write_u32(line, 10);
  debug_write_line("");

  /* Infinite loop */
  while (1) {
  }
}

CCIF unsigned long clock_seconds() {
  return time_ms() / 1000;
}

clock_time_t clock_time(void) {
  return time_ms();
}
