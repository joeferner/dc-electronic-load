
#ifndef _SST25FLASH_H_
#define _SST25FLASH_H_

#include "platform_config.h"

#ifdef FLASH_ENABLE

void sst25flash_setup();
void sst25flash_erase_4k(uint32_t addr);
void sst25flash_erase_32k(uint32_t addr);
void sst25flash_erase_64k(uint32_t addr);
void sst25flash_erase_chip();
void sst25flash_read_begin(uint32_t addr);
void sst25flash_read_end();
uint8_t sst25flash_read();
void sst25flash_write_enable();
void sst25flash_write_byte(uint32_t addr, uint8_t val);
void sst25flash_write_status_reg(uint8_t val);
uint8_t sst25flash_read_status_reg();
void sst25flash_read_id(uint8_t* manufacturerId, uint8_t* deviceId);

#endif

#endif