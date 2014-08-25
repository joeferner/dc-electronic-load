#ifndef _ENCODER_H_
#define _ENCODER_H_

typedef enum {
  ENCODER_DIR_CW,
  ENCODER_DIR_CCW
} ENCODER_DIR;

extern void encoder_irq(ENCODER_DIR dir);

void encoder_setup();
void encoder_exti_irq();

#endif // _ENCODER_H_
