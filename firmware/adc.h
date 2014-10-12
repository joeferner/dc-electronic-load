
#ifndef _ADC_H_
#define _ADC_H_

#include "sys/process.h"
#include <stdint.h>
#include "platform_config.h"

#ifdef ADC_ENABLE

#define ADC_CH0_SINGLE  0x0600
#define ADC_CH1_SINGLE  0x0640
#define ADC_CH2_SINGLE  0x0680
#define ADC_CH3_SINGLE  0x06c0

PROCESS_NAME(adc_volts_process);
PROCESS_NAME(adc_current_process);
PROCESS_NAME(adc_temp_process);

void adc_setup();
uint16_t adc_sample(uint16_t ch);
extern void adc_irq(uint8_t channel, uint16_t value);

#endif

#endif // _ADC_H_
