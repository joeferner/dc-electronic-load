
#include "mac25aa02e48.h"
#include "platform_config.h"
#include "debug.h"

#ifdef MAC_ENABLE

#define EUI48_ADDRESS_START 0xfa
#define EUI48_ADDRESS_END   0xff

#define MAC25AA02E48_OPCODE_READ        0x03
#define MAC25AA02E48_OPCODE_READ_STATUS 0x05

uint8_t EUI48[EUI48_LENGTH];

void mac25aa02e48_spi_assert();
void mac25aa02e48_spi_deassert();
uint8_t mac25aa02e48_spi_transfer(uint8_t d);

void mac25aa02e48_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?mac25aa02e48_setup");
  RCC_APB2PeriphClockCmd(MAC_CS_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  mac25aa02e48_spi_deassert();
  gpioInitStructure.GPIO_Pin = MAC_CS_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MAC_CS_PORT, &gpioInitStructure);
  mac25aa02e48_spi_deassert();
}

void mac25aa02e48_spi_assert() {
  GPIO_ResetBits(MAC_CS_PORT, MAC_CS_PIN);
}

void mac25aa02e48_spi_deassert() {
  GPIO_SetBits(MAC_CS_PORT, MAC_CS_PIN);
}

uint8_t mac25aa02e48_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(MAC_SPI, d);
  while (SPI_I2S_GetFlagStatus(MAC_SPI, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(MAC_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(MAC_SPI, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(MAC_SPI);
}

void mac25aa02e48_read_begin(uint8_t addr) {
  mac25aa02e48_spi_assert();
  mac25aa02e48_spi_transfer(MAC25AA02E48_OPCODE_READ);
  mac25aa02e48_spi_transfer(addr);
}

uint8_t mac25aa02e48_read() {
  return mac25aa02e48_spi_transfer(0x00);
}

void mac25aa02e48_read_end() {
  mac25aa02e48_spi_deassert();
}

uint8_t mac25aa02e48_read_addr(uint8_t addr) {
  uint8_t d;
  mac25aa02e48_read_begin(addr);
  d = mac25aa02e48_read();
  mac25aa02e48_read_end();
  return d;
}

void mac25aa02e48_read_eui48() {
  int i;
  
  mac25aa02e48_read_begin(EUI48_ADDRESS_START);
  for (i = 0; i < EUI48_LENGTH; i++) {
    EUI48[i] = mac25aa02e48_read();
  }
  mac25aa02e48_read_end();

  debug_write("EUI48: ");
  for (i = 0; i < EUI48_LENGTH; i++) {
    if (i != 0) {
      debug_write(":");
    }
    debug_write_u8(EUI48[i], 16);
  }
  debug_write_line("");
}

uint8_t mac25aa02e48_read_status() {
  uint8_t status;
  
  mac25aa02e48_spi_assert();
  
  mac25aa02e48_spi_transfer(MAC25AA02E48_OPCODE_READ_STATUS);
  status = mac25aa02e48_spi_transfer(0x00);
  
  mac25aa02e48_spi_deassert();
  
  return status;
}

#endif
