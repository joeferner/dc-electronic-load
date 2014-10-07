
#include <stm32f10x_spi.h>
#include "platform_config.h"
#include "debug.h"

void spi_setup() {
  SPI_InitTypeDef spiInitStruct;
  GPIO_InitTypeDef gpioConfig;

#ifdef SPI1_ENABLE
  debug_write_line("?spi1_setup");
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

  // Configure SPI1 pins: SCK (pin 5) and MOSI (pin 7)
  gpioConfig.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpioConfig);

  // Configure SPI1 pins: MISO (pin 6)
  gpioConfig.GPIO_Pin = GPIO_Pin_6;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpioConfig);

  // init SPI
  SPI_StructInit(&spiInitStruct);
  spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiInitStruct.SPI_Mode = SPI_Mode_Master;
  spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  spiInitStruct.SPI_NSS = SPI_NSS_Soft;
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 0 (CPOL = 0, CPHA = 0)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;

  SPI_Init(SPI1, &spiInitStruct);

  SPI_Cmd(SPI1, ENABLE);
#endif

#ifdef SPI2_ENABLE
  debug_write_line("?spi2_setup");
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // Configure SPI2 pins: SCK (pin 13) and MOSI (pin 15)
  gpioConfig.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpioConfig);

  // Configure SPI2 pins: MISO (pin 14)
  gpioConfig.GPIO_Pin = GPIO_Pin_14;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &gpioConfig);

  // init SPI
  SPI_StructInit(&spiInitStruct);
  spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiInitStruct.SPI_Mode = SPI_Mode_Master;
  spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  spiInitStruct.SPI_NSS = SPI_NSS_Soft;
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 0 (CPOL = 0, CPHA = 0)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;

  SPI_Init(SPI2, &spiInitStruct);

  SPI_Cmd(SPI2, ENABLE);
#endif
}
