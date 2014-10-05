
#include "platform_config.h"
#include "sst25flash.h"
#include "debug.h"
#include "delay.h"

#ifdef FLASH_ENABLE

#define SST25FLASH_OP_WRITE_STATUS_REG        0x01
#define SST25FLASH_OP_WRITE_BYTE              0x02
#define SST25FLASH_OP_READ                    0x03
#define SST25FLASH_OP_READ_STATUS_REG         0x05
#define SST25FLASH_OP_WRITE_ENABLE            0x06
#define SST25FLASH_OP_ERASE_4K                0x20
#define SST25FLASH_OP_ENABLE_WRITE_STATUS_REG 0x50
#define SST25FLASH_OP_ERASE_32K               0x52
#define SST25FLASH_OP_ERASE_CHIP              0x60
#define SST25FLASH_OP_READ_ID                 0x90
#define SST25FLASH_OP_ERASE_64K               0xd8

#define SST25FLASH_STATUS_REG_BUSY 0x01

void sst25flash_spi_assert();
void sst25flash_spi_deassert();
uint8_t sst25flash_spi_transfer(uint8_t d);
void sst25flash_spi_transfer_addr(uint32_t addr);
void sst25flash_poll_until_write_complete();

void sst25flash_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?sst25flash_setup");
  RCC_APB2PeriphClockCmd(FLASH_CS_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  sst25flash_spi_deassert();
  gpioInitStructure.GPIO_Pin = FLASH_CS_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FLASH_CS_PORT, &gpioInitStructure);
  sst25flash_spi_deassert();
}

void sst25flash_spi_assert() {
  GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN);
}

void sst25flash_spi_deassert() {
  GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN);
}

uint8_t sst25flash_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(FLASH_SPI, d);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(FLASH_SPI);
}

void sst25flash_spi_transfer_addr(uint32_t addr) {
  sst25flash_spi_transfer((addr >> 16) & 0xff);
  sst25flash_spi_transfer((addr >> 8) & 0xff);
  sst25flash_spi_transfer((addr >> 0) & 0xff);
}

uint8_t sst25flash_read_status_reg() {
  uint8_t r;

  sst25flash_spi_assert();

  sst25flash_spi_transfer(SST25FLASH_OP_READ_STATUS_REG);
  r = sst25flash_spi_transfer(0x00);

  sst25flash_spi_deassert();

  return r;
}

void sst25flash_poll_until_write_complete() {
  uint8_t r;

  sst25flash_spi_assert();

  sst25flash_spi_transfer(SST25FLASH_OP_READ_STATUS_REG);
  while (1) {
    r = sst25flash_spi_transfer(0x00);
    if ((r & SST25FLASH_STATUS_REG_BUSY) == 0x00) {
      break;
    }
  }

  sst25flash_spi_deassert();
}

void sst25flash_write_status_reg(uint8_t val) {
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_ENABLE_WRITE_STATUS_REG);
  sst25flash_spi_deassert();

  delay_us(100);

  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_WRITE_STATUS_REG);
  sst25flash_spi_transfer(val);
  sst25flash_spi_deassert();
}

void sst25flash_write_enable() {
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_WRITE_ENABLE);
  sst25flash_spi_deassert();
}

void sst25flash_erase_4k(uint32_t addr) {
  sst25flash_write_enable();
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_ERASE_4K);
  sst25flash_spi_transfer_addr(addr);
  sst25flash_spi_deassert();

  sst25flash_poll_until_write_complete();
}

void sst25flash_erase_32k(uint32_t addr) {
  sst25flash_write_enable();
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_ERASE_32K);
  sst25flash_spi_transfer_addr(addr);
  sst25flash_spi_deassert();

  sst25flash_poll_until_write_complete();
}

void sst25flash_erase_64k(uint32_t addr) {
  sst25flash_write_enable();
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_ERASE_64K);
  sst25flash_spi_transfer_addr(addr);
  sst25flash_spi_deassert();

  sst25flash_poll_until_write_complete();
}

void sst25flash_erase_chip() {
  sst25flash_write_enable();
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_ERASE_CHIP);
  sst25flash_spi_deassert();

  sst25flash_poll_until_write_complete();
}

void sst25flash_read_begin(uint32_t addr) {
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_READ);
  sst25flash_spi_transfer_addr(addr);
}

void sst25flash_read_end() {
  sst25flash_spi_deassert();
}

uint8_t sst25flash_read() {
  return sst25flash_spi_transfer(0x00);
}

void sst25flash_write_byte(uint32_t addr, uint8_t val) {
  sst25flash_write_enable();
  sst25flash_spi_assert();
  sst25flash_spi_transfer(SST25FLASH_OP_WRITE_BYTE);
  sst25flash_spi_transfer_addr(addr);
  sst25flash_spi_transfer(val);
  sst25flash_spi_deassert();

  sst25flash_poll_until_write_complete();
}

void sst25flash_read_id(uint8_t* manufacturerId, uint8_t* deviceId) {
  sst25flash_spi_assert();

  sst25flash_spi_transfer(SST25FLASH_OP_READ_ID);
  sst25flash_spi_transfer_addr(0x00000);
  *manufacturerId = sst25flash_spi_transfer(0x00);
  *deviceId = sst25flash_spi_transfer(0x00);

  sst25flash_spi_deassert();
}

#endif
