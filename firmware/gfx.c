#include "gfx.h"
#include "disp6800.h"
#include "debug.h"
#include "font.h"
#include "fontLarge.c"
#include "fontXSmall.c"
#include "fontSmallNumbers.c"
#include <string.h>

#ifdef DISP6800_ENABLE

#define DISP6800_COLUMN_START 0x00
#define DISP6800_COLUMN_END   0x40
#define DISP6800_COLUMNS      (DISP6800_COLUMN_END - DISP6800_COLUMN_START)
#define DISP6800_ROW_START    0x00
#define DISP6800_ROW_END      0x50
#define DISP6800_ROWS         (DISP6800_ROW_END - DISP6800_ROW_START)
#define DISP6800_VBUF_SIZE    (DISP6800_COLUMNS * DISP6800_ROWS)

#define VBUF_OFFSET(x, y)     (((y) * DISP6800_COLUMNS) + ((x) / 2))
#define VBUF_LOW_HIGH(x, y)   ((x) % 2)
#define MIN(x, y)             (((x) < (y)) ? (x) : (y))
#define MAX(x, y)             (((x) > (y)) ? (x) : (y))
#define CLAMP(x, min, max)    (MAX(MIN(x, max), min))
#define TEST(a, b)            (((a) & (b)) == (b))

uint8_t vbuf[DISP6800_VBUF_SIZE];

void gfx_setup() {
  debug_write_line("?BEGIN gfx_setup");

  gfx_clear();
  gfx_redraw();

  debug_write_line("?END gfx_setup");
}

void gfx_clear() {
  for (int i = 0; i < DISP6800_VBUF_SIZE; i++) {
    vbuf[i] = 0;
  }
}

void gfx_redraw() {
  disp6800_set_column_address(DISP6800_COLUMN_START, DISP6800_COLUMN_END - 1);
  disp6800_set_row_address(DISP6800_ROW_START, DISP6800_ROW_END - 1);

  disp6800_begin_tx_data();
  for (int i = 0; i < DISP6800_VBUF_SIZE; i++) {
    disp6800_tx_data(vbuf[i]);
  }
}

void gfx_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
  int vbufOffset = CLAMP(VBUF_OFFSET(x, y), 0, DISP6800_VBUF_SIZE);
  int lh = VBUF_LOW_HIGH(x, y);
  if (lh) {
    vbuf[vbufOffset] = (vbuf[vbufOffset] & 0x0f) | ((color & 0x0f) << 4);
  } else {
    vbuf[vbufOffset] = (vbuf[vbufOffset] & 0xf0) | ((color & 0x0f) << 0);
  }
}

int gfx_measure_string_width(const char* str, const tFont* font) {
  int width = 0;
  while (*str) {
    char ch = *str++;
    const tChar* tch = gfx_get_char(ch, font);
    if (tch != NULL) {
      width += tch->image->width;
    }
  }
  return width;
}

int gfx_draw_string(const char* str, const tFont* font, int x, int y, uint32_t opts) {
  if (TEST(opts, GFX_ALIGN_RIGHT)) {
    x -= gfx_measure_string_width(str, font);
  } else if (TEST(opts, GFX_ALIGN_CENTER)) {
    x -= gfx_measure_string_width(str, font) / 2;
  }

  int cx = 0;
  while (*str) {
    char ch = *str++;
    cx += gfx_draw_char(ch, font, x + cx, y);
  }
  return cx;
}

const tChar* gfx_get_char(char ch, const tFont* font) {
  for (int i = 0; i < font->length; i++) {
    const tChar* tchar = &font->chars[i];
    if (tchar->code == ch) {
      return tchar;
    }
  }
  return NULL;
}

int gfx_draw_char(char ch, const tFont* font, int x, int y) {
  const tChar* tchar = gfx_get_char(ch, font);
  if (tchar == NULL) {
    return 0;
  }
  const tImage* image = tchar->image;

  int imageOffset = 0;
  int ly, lx, b;
  uint8_t color;
  for (ly = 0; ly < image->height; ly++) {
    for (lx = 0; lx < image->width;) {
      for (b = 7; b >= 0; b--, lx++) {
        color = (image->data[imageOffset] & (1 << b)) ? 0xf : 0x0;
        gfx_set_pixel(x + lx, y + ly, color);
      }
      imageOffset++;
    }
  }

  return image->width;
}

#endif
