#include "dac.h"
#include "debug.h"
#include "platform_config.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#ifdef DAC_ENABLE

#define MCP4901_BUF             0x40
#define MCP4901_GAIN_1X         0x20
#define MCP4901_SHUTDOWN_FALSE  0x10

void dac_spi_assert();
void dac_spi_deassert();
uint8_t dac_spi_transfer(uint8_t d);

void dac_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?dac_setup");
  RCC_APB2PeriphClockCmd(DAC_CS_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  dac_spi_deassert();
  gpioInitStructure.GPIO_Pin = DAC_CS_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DAC_CS_PORT, &gpioInitStructure);
  dac_spi_deassert();
}

void dac_set(uint16_t value) {
  uint8_t h = (value >> 8) & 0x0f;
  uint8_t l = value & 0xff;
  
  dac_spi_assert();
  dac_spi_transfer(MCP4901_BUF | MCP4901_GAIN_1X | MCP4901_SHUTDOWN_FALSE | h);
  dac_spi_transfer(l);
  dac_spi_deassert();
}

void dac_spi_assert() {
  GPIO_ResetBits(DAC_CS_PORT, DAC_CS_PIN);
}
  
void dac_spi_deassert() {
  GPIO_SetBits(DAC_CS_PORT, DAC_CS_PIN);
}

uint8_t dac_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(DAC_SPI, d);
  while (SPI_I2S_GetFlagStatus(DAC_SPI, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(DAC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(DAC_SPI, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(DAC_SPI);
}

#endif