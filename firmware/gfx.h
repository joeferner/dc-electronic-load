
#ifndef _GFX_H_
#define _GFX_H_

#include <stdint.h>
#include "font.h"

#define GFX_ALIGN_LEFT  0x00000000
#define GFX_ALIGN_RIGHT 0x00000001

void gfx_setup();
void gfx_clear();
void gfx_set_pixel(uint8_t x, uint8_t y, uint8_t color);
void gfx_redraw();
int gfx_draw_string(const char* str, const tFont* font, int x, int y, uint32_t opts);
int gfx_draw_char(char ch, const tFont* font, int x, int y);
int gfx_measure_string_width(const char* str, const tFont* font);
const tChar* gfx_get_char(char ch, const tFont* font);

#endif // _GFX_H_
