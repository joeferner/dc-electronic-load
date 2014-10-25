#include "adc.h"
#include "debug.h"
#include "platform_config.h"
#include "delay.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#ifdef ADC_ENABLE

PROCESS(adc_volts_process, "ADC Volts");
PROCESS(adc_current_process, "ADC Current");
PROCESS(adc_temp_process, "ADC Temp");

struct etimer adc_volts_etimer;
struct etimer adc_current_etimer;
struct etimer adc_temp_etimer;

void adc_spi_assert();
void adc_spi_deassert();
uint8_t adc_spi_transfer(uint8_t d);

void adc_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?adc_setup");
  RCC_APB2PeriphClockCmd(ADC_CS_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  adc_spi_deassert();
  gpioInitStructure.GPIO_Pin = ADC_CS_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADC_CS_PORT, &gpioInitStructure);
  adc_spi_deassert();

  process_start(&adc_volts_process, NULL);
  process_start(&adc_current_process, NULL);
  process_start(&adc_temp_process, NULL);
}

void adc_spi_assert() {
  GPIO_ResetBits(ADC_CS_PORT, ADC_CS_PIN);
}

void adc_spi_deassert() {
  GPIO_SetBits(ADC_CS_PORT, ADC_CS_PIN);
}

uint8_t adc_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(ADC_SPI, d);
  while (SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(ADC_SPI);
}

uint16_t adc_sample(uint16_t ch) {
  adc_spi_assert();
  adc_spi_transfer((ch >> 8) & 0xff);
  uint16_t high = (adc_spi_transfer(ch & 0xff) & 0x0f);
  uint16_t low = adc_spi_transfer(0x00);
  uint16_t value = (high << 8) | low;
  adc_spi_deassert();
  return value;
}

PROCESS_THREAD(adc_volts_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&adc_volts_etimer, CLOCK_SECOND / 10);

  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&adc_volts_etimer));
    uint16_t value = adc_sample(ADC_CH0_SINGLE);
    adc_irq(0, value);

    etimer_reset(&adc_volts_etimer);
  }

  PROCESS_END();
}

PROCESS_THREAD(adc_current_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&adc_current_etimer, CLOCK_SECOND / 10);

  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&adc_current_etimer));
    uint16_t value = adc_sample(ADC_CH1_SINGLE);
    adc_irq(1, value);

    etimer_reset(&adc_current_etimer);
  }

  PROCESS_END();
}

PROCESS_THREAD(adc_temp_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&adc_temp_etimer, CLOCK_SECOND);

  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&adc_temp_etimer));
    uint16_t value = adc_sample(ADC_CH2_SINGLE);
    adc_irq(2, value);
    value = adc_sample(ADC_CH3_SINGLE);
    adc_irq(3, value);

    etimer_reset(&adc_temp_etimer);
  }

  PROCESS_END();
}

#endif



