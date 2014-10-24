
#ifndef _DC_ELECTRONIC_LOAD_H_
#define _DC_ELECTRONIC_LOAD_H_

#include "platform_config.h"
#include "sys/process.h"
#include <stdint.h>
#include <stdlib.h>
#include "ring_buffer.h"
#include "time.h"
#include "spi.h"
#include "debug.h"
#ifdef DISP6800_ENABLE
#include "disp6800.h"
#include "gfx.h"
#endif
#ifdef ENCODER_ENABLE
#include "encoder.h"
#endif
#ifdef FAN_ENABLE
#include "fan.h"
#endif
#ifdef ADC_ENABLE
#include "adc.h"
#endif
#ifdef DAC_ENABLE
#include "dac.h"
#endif
#ifdef FLASH_ENABLE
#include "flashsst25.h"
#endif
#ifdef MAC_ENABLE
#include "mac25aa02e48.h"
#endif
#ifdef NETWORK_ENABLE
#include "network.h"
#endif

PROCESS_NAME(debug_process);

#ifdef DISP6800_ENABLE
PROCESS_NAME(gfx_update_process);
#endif

void dc_electronic_load_setup();
void dc_electronic_load_loop();
void set_current_milliamps(uint32_t value);
uint16_t get_millivolts();
uint16_t get_milliamps();
uint16_t get_set_milliamps();

#endif
