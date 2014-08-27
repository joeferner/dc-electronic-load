#include "adc.h"
#include "debug.h"
#include "platform_config.h"

#ifdef ADC_ENABLE
PROCESS(adc_process, "ADC");

struct etimer adc_etimer;

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

  process_start(&adc_process, NULL);
}

void adc_spi_assert() {
  GPIO_ResetBits(ADC_CS_PORT, ADC_CS_PIN);
}

void adc_spi_deassert() {
  GPIO_SetBits(ADC_CS_PORT, ADC_CS_PIN);
}

uint8_t adc_spi_transfer(uint8_t d) {
  while (SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(ADC_SPI, d);
  while (SPI_I2S_GetFlagStatus(ADC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(ADC_SPI);
}

PROCESS_THREAD(adc_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&adc_etimer, CLOCK_SECOND / 10);

  while (1) {
    PROCESS_YIELD();
    adc_spi_assert();
    adc_spi_transfer(0x06);
    uint16_t high = adc_spi_transfer(0x00);
    uint16_t low = adc_spi_transfer(0x00);
    uint16_t value = (high << 8) | low;
    adc_irq(0, value);
    adc_spi_deassert();

    etimer_reset(&adc_etimer);
  }

  PROCESS_END();
}
#endif



