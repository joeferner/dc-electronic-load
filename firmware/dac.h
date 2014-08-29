
#ifndef _DAC_H_
#define _DAC_H_

#include "sys/process.h"
#include <stdint.h>
#include "platform_config.h"

#ifdef DAC_ENABLE

void dac_setup();
void dac_set(uint16_t value);

#endif

#endif
