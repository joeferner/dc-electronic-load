
#include "gfx.h"
#include "disp6800.h"
#include "debug.h"

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

uint8_t vbuf[DISP6800_VBUF_SIZE];

void gfx_setup() {
  int x;
  debug_write_line("?BEGIN gfx_setup");

  for(int i = 0; i < DISP6800_VBUF_SIZE; i++) {
      vbuf[i] = 0;
  }

  vbuf[10 + (0 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (1 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (2 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (3 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (4 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (5 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (6 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (7 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (8 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (9 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (10 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (11 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (12 * DISP6800_COLUMNS)] = 0xff;
  vbuf[10 + (13 * DISP6800_COLUMNS)] = 0xcf;
  vbuf[11 + (13 * DISP6800_COLUMNS)] = 0x8a;
  vbuf[12 + (13 * DISP6800_COLUMNS)] = 0x46;
  vbuf[13 + (13 * DISP6800_COLUMNS)] = 0x02;

  for(x = 0; x < 20; x++) {
    gfx_set_pixel(x, x, 0xf);
  }

  for(x = 22; x < 42; x++) {
    gfx_set_pixel(64, x, 0xf);
  }
  for(x = 54; x < 74; x++) {
    gfx_set_pixel(x, 32, 0xf);
  }
  for(x = 5; x < 120; x++) {
    gfx_set_pixel(x, 20, 0xf);
  }
  for(x = 8; x < 120; x++) {
    gfx_set_pixel(x, 21, 0xf);
  }

  gfx_redraw();

  debug_write_line("?END gfx_setup");
}

void gfx_redraw() {
  /*
  int i = 0;
  for(int y = 0; y < DISP6800_ROWS; y++) {
    disp6800_set_column_address(DISP6800_COLUMN_START, DISP6800_COLUMN_END - 1);
    disp6800_set_row_address(y, DISP6800_ROW_END - 1);

    disp6800_begin_tx_data();
    for(int x = 0; x < DISP6800_COLUMNS; x++) {
      uint8_t d = vbuf[i++];
      if()
      disp6800_tx_data(d);
    }
  }
  */

  disp6800_set_column_address(DISP6800_COLUMN_START, DISP6800_COLUMN_END - 1);
  disp6800_set_row_address(DISP6800_ROW_START, DISP6800_ROW_END - 1);

  disp6800_begin_tx_data();
  for(int i = 0; i < DISP6800_VBUF_SIZE; i++) {
    disp6800_tx_data(vbuf[i]);
  }
}

void gfx_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
  int vbufOffset = CLAMP(VBUF_OFFSET(x, y), 0, DISP6800_VBUF_SIZE);
  int lh = VBUF_LOW_HIGH(x, y);
  if(lh) {
    vbuf[vbufOffset] = (vbuf[vbufOffset] & 0x0f) | ((color & 0x0f) << 4);
  } else {
    vbuf[vbufOffset] = (vbuf[vbufOffset] & 0xf0) | ((color & 0x0f) << 0);
  }
}
