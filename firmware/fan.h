
#ifndef _FAN_H_
#define _FAN_H_

#include <stdint.h>
#include "platform_config.h"

#ifdef FAN_ENABLE

void fan_setup();
void fan_set(uint8_t percent);
uint8_t fan_get();

#endif

#endif
