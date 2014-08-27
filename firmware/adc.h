
#ifndef _ADC_H_
#define _ADC_H_

#include "sys/process.h"
#include <stdint.h>

PROCESS_NAME(adc_process);

void adc_setup();
extern void adc_irq(uint8_t channel, uint16_t value);

#endif // _ADC_H_
