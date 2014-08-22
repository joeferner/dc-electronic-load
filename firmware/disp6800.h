
#ifndef _DISP6800_H_
#define _DISP6800_H_

#define DISP6800_DISPLAY_OFF   0xae
#define DISP6800_DISPLAY_ON    0xaf

void disp6800_setup();
void disp6800_set_display_onoff(uint8_t onoff);

#endif // _DISP6800_H_
