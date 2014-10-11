
#ifndef _25AA02E48MAC_H_
#define _25AA02E48MAC_H_

#include "platform_config.h"
#include <stdint.h>

#ifdef MAC_ENABLE

#define EUI48_LENGTH 6
extern uint8_t EUI48[EUI48_LENGTH];

void mac25aa02e48_setup();
void mac25aa02e48_read_eui48();
void mac25aa02e48_read_begin(uint8_t addr);
uint8_t mac25aa02e48_read();
uint8_t mac25aa02e48_read_addr(uint8_t addr);
void mac25aa02e48_read_end();
uint8_t mac25aa02e48_read_status();

#endif

#endif