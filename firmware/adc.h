
#ifndef _ADC_H_
#define _ADC_H_

#include "sys/process.h"
#include <stdint.h>
#include "platform_config.h"

#ifdef ADC_ENABLE

PROCESS_NAME(adc_volts_process);
PROCESS_NAME(adc_current_process);
PROCESS_NAME(adc_temp_process);

void adc_setup();
extern void adc_irq(uint8_t channel, uint16_t value);

#endif

#endif // _ADC_H_
