
#ifndef _SST25FLASH_H_
#define _SST25FLASH_H_

#include "platform_config.h"

#ifdef FLASH_ENABLE

#define FLASH_BLOCK_SIZE 512

void flashsst25_setup();
void flashsst25_erase_4k(uint32_t addr);
void flashsst25_erase_32k(uint32_t addr);
void flashsst25_erase_64k(uint32_t addr);
void flashsst25_erase_chip();
void flashsst25_read_begin(uint32_t addr);
void flashsst25_read_end();
uint8_t flashsst25_read();
void flashsst25_readn(uint32_t addr, uint8_t* buffer, uint16_t len);
void flashsst25_write_enable();
void flashsst25_write(uint32_t addr, uint8_t* buffer, uint16_t len);
void flashsst25_write_byte(uint32_t addr, uint8_t val);
void flashsst25_write_status_reg(uint8_t val);
uint8_t flashsst25_read_status_reg();
void flashsst25_read_id(uint8_t* manufacturerId, uint8_t* deviceId);

#endif

#endif