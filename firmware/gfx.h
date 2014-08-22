
#ifndef _GFX_H_
#define _GFX_H_

#include <stdint.h>

void gfx_setup();
void gfx_set_pixel(uint8_t x, uint8_t y, uint8_t color);
void gfx_redraw();

#endif // _GFX_H_
