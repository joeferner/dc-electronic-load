
#include "platform_config.h"
#include "flashsst25.h"
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

void flashsst25_spi_assert();
void flashsst25_spi_deassert();
uint8_t flashsst25_spi_transfer(uint8_t d);
void flashsst25_spi_transfer_addr(uint32_t addr);
void flashsst25_poll_until_write_complete();

void flashsst25_setup() {
  GPIO_InitTypeDef gpioInitStructure;

  debug_write_line("?flashsst25_setup");
  RCC_APB2PeriphClockCmd(FLASH_CS_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  flashsst25_spi_deassert();
  gpioInitStructure.GPIO_Pin = FLASH_CS_PIN;
  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FLASH_CS_PORT, &gpioInitStructure);
  flashsst25_spi_deassert();

  flashsst25_write_status_reg(0x00);
}

void flashsst25_spi_assert() {
  GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN);
}

void flashsst25_spi_deassert() {
  GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN);
}

uint8_t flashsst25_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(FLASH_SPI, d);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(FLASH_SPI);
}

void flashsst25_spi_transfer_addr(uint32_t addr) {
  flashsst25_spi_transfer((addr >> 16) & 0xff);
  flashsst25_spi_transfer((addr >> 8) & 0xff);
  flashsst25_spi_transfer((addr >> 0) & 0xff);
}

uint8_t flashsst25_read_status_reg() {
  uint8_t r;

  flashsst25_spi_assert();

  flashsst25_spi_transfer(SST25FLASH_OP_READ_STATUS_REG);
  r = flashsst25_spi_transfer(0x00);

  flashsst25_spi_deassert();

  return r;
}

void flashsst25_poll_until_write_complete() {
  uint8_t r;

  flashsst25_spi_assert();

  flashsst25_spi_transfer(SST25FLASH_OP_READ_STATUS_REG);
  while (1) {
    r = flashsst25_spi_transfer(0x00);
    if ((r & SST25FLASH_STATUS_REG_BUSY) == 0x00) {
      break;
    }
  }

  flashsst25_spi_deassert();
}

void flashsst25_write_status_reg(uint8_t val) {
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_ENABLE_WRITE_STATUS_REG);
  flashsst25_spi_deassert();

  delay_us(100);

  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_WRITE_STATUS_REG);
  flashsst25_spi_transfer(val);
  flashsst25_spi_deassert();
}

void flashsst25_write_enable() {
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_WRITE_ENABLE);
  flashsst25_spi_deassert();
}

void flashsst25_erase_4k(uint32_t addr) {
  flashsst25_write_enable();
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_ERASE_4K);
  flashsst25_spi_transfer_addr(addr);
  flashsst25_spi_deassert();

  flashsst25_poll_until_write_complete();
}

void flashsst25_erase_32k(uint32_t addr) {
  flashsst25_write_enable();
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_ERASE_32K);
  flashsst25_spi_transfer_addr(addr);
  flashsst25_spi_deassert();

  flashsst25_poll_until_write_complete();
}

void flashsst25_erase_64k(uint32_t addr) {
  flashsst25_write_enable();
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_ERASE_64K);
  flashsst25_spi_transfer_addr(addr);
  flashsst25_spi_deassert();

  flashsst25_poll_until_write_complete();
}

void flashsst25_erase_chip() {
  flashsst25_write_enable();
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_ERASE_CHIP);
  flashsst25_spi_deassert();

  flashsst25_poll_until_write_complete();
}

void flashsst25_read_begin(uint32_t addr) {
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_READ);
  flashsst25_spi_transfer_addr(addr);
}

void flashsst25_read_end() {
  flashsst25_spi_deassert();
}

uint8_t flashsst25_read() {
  return flashsst25_spi_transfer(0x00);
}

void flashsst25_readn(uint32_t addr, uint8_t* buffer, uint16_t len) {
  uint16_t i;
  uint8_t* o = buffer;
  flashsst25_read_begin(addr);
  for (i = 0; i < len; i++) {
    *o++ = flashsst25_read();
  }
  flashsst25_read_end();
}

// TODO this could speed up with a bulk write
void flashsst25_write(uint32_t addr, uint8_t* buffer, uint16_t len) {
  uint16_t i;
  for (i = 0; i < len; i++, addr++) {
    flashsst25_write_byte(addr, buffer[i]);
  }
}

void flashsst25_write_byte(uint32_t addr, uint8_t val) {
  flashsst25_write_enable();
  flashsst25_spi_assert();
  flashsst25_spi_transfer(SST25FLASH_OP_WRITE_BYTE);
  flashsst25_spi_transfer_addr(addr);
  flashsst25_spi_transfer(val);
  flashsst25_spi_deassert();

  flashsst25_poll_until_write_complete();
}

void flashsst25_read_id(uint8_t* manufacturerId, uint8_t* deviceId) {
  flashsst25_spi_assert();

  flashsst25_spi_transfer(SST25FLASH_OP_READ_ID);
  flashsst25_spi_transfer_addr(0x00000);
  *manufacturerId = flashsst25_spi_transfer(0x00);
  *deviceId = flashsst25_spi_transfer(0x00);

  flashsst25_spi_deassert();
}

#endif
