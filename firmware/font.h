#ifndef FONT_H
#define FONT_H

typedef struct {
  const uint8_t* data;
  int width;
  int height;
} tImage;

typedef struct {
  int code;
  const tImage *image;
} tChar;

typedef struct {
  int length;
  const tChar *chars;
} tFont;

extern const tFont FONT_LARGE;

#endif // FONT_H
