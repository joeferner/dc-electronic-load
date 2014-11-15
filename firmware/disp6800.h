
#ifndef _DISP6800_H_
#define _DISP6800_H_

#include "platform_config.h"

#ifdef DISP6800_ENABLE

#define DISP6800_DISPLAY_OFF   0xae
#define DISP6800_DISPLAY_ON    0xaf

void disp6800_setup();
void disp6800_set_display_onoff(uint8_t onoff);
void disp6800_set_column_address(uint8_t start, uint8_t end);
void disp6800_set_row_address(uint8_t start, uint8_t end);
void disp6800_begin_tx_data();
void disp6800_end_tx_data();
void disp6800_tx_data(uint8_t d);

#endif

#endif // _DISP6800_H_
