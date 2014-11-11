
#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "platform_config.h"
#include <stdint.h>

#ifdef BUTTONS_ENABLE

#define BUTTON_A 0x01
#define BUTTON_B 0x02
#define BUTTON_C 0x04
#define BUTTON_D 0x08

extern void buttons_irq(uint8_t buttons);
void buttons_setup();
void buttons_loop();

#endif

#endif