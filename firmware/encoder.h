#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "platform_config.h"

#ifdef ENCODER_ENABLE

typedef enum {
  ENCODER_DIR_CW,
  ENCODER_DIR_CCW
} ENCODER_DIR;

extern void encoder_irq(ENCODER_DIR dir);

void encoder_setup();
void encoder_exti_irq();

#endif

#endif // _ENCODER_H_
