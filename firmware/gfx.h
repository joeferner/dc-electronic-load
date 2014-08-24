
#ifndef _GFX_H_
#define _GFX_H_

#include <stdint.h>
#include "font.h"

void gfx_setup();
void gfx_set_pixel(uint8_t x, uint8_t y, uint8_t color);
void gfx_redraw();
int gfx_draw_string(const char* str, const tFont* font, int x, int y);
int gfx_draw_char(char ch, const tFont* font, int x, int y);
const tChar* gfx_get_char(char ch, const tFont* font);

#endif // _GFX_H_
