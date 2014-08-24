
/*******************************************************************************
* font
* filename: /home/jferner/dev/dc-electronic-load/firmware/fontLarge.xml
* name: FONT_LARGE
* family: Ubuntu
* size: 36
* style: Medium
* included characters: 0123456789,.
* antialiasing: yes
* type: monospaced
* encoding: UTF-8
* unicode bom: no
*
* preset name: Grayscale 8
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Grayscale, not_used not_used
* bits per pixel: 8
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: yes
*******************************************************************************/

/*
 typedef struct {
     long int code;
     const tImage *image;
     } tChar;
 typedef struct {
     int length;
     const tChar *chars;
     } tFont;
*/

#include <stdint.h>



static const uint8_t image_data_FONT_LARGE_0x30[330] = {
    0x00, 0x00, 0x00, 0x00, 0x64, 0x57, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xcf, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0xd1, 0xff, 0xff, 0xff, 0xff, 0x6f, 0x00, 0x00, 
    0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0x70, 0xff, 0xff, 0x5c, 0x84, 0xff, 0xff, 0x0e, 0x00, 
    0x00, 0xf1, 0xff, 0xbf, 0x00, 0x00, 0xf5, 0xff, 0x7f, 0x00, 
    0x00, 0xf6, 0xff, 0x2f, 0x00, 0x00, 0xb0, 0xff, 0xdf, 0x00, 
    0x00, 0xfb, 0xff, 0x0b, 0x00, 0x00, 0x50, 0xff, 0xff, 0x02, 
    0x00, 0xff, 0xff, 0x07, 0x00, 0x00, 0x10, 0xff, 0xff, 0x06, 
    0x20, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x09, 
    0x40, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x0b, 
    0x50, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x0c, 
    0x60, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x0d, 
    0x70, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xff, 0x0e, 
    0x60, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x0d, 
    0x50, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 0xfb, 0xff, 0x0c, 
    0x40, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x0b, 
    0x20, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x09, 
    0x00, 0xff, 0xff, 0x07, 0x00, 0x00, 0x10, 0xff, 0xff, 0x06, 
    0x00, 0xfb, 0xff, 0x0b, 0x00, 0x00, 0x50, 0xff, 0xff, 0x02, 
    0x00, 0xf6, 0xff, 0x2f, 0x00, 0x00, 0xb0, 0xff, 0xdf, 0x00, 
    0x00, 0xf1, 0xff, 0xbf, 0x00, 0x00, 0xf5, 0xff, 0x7f, 0x00, 
    0x00, 0x80, 0xff, 0xff, 0x5c, 0x84, 0xff, 0xff, 0x0e, 0x00, 
    0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0x00, 0xd1, 0xff, 0xff, 0xff, 0xff, 0x6f, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf9, 0xff, 0xff, 0xcf, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x64, 0x57, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x30 = { image_data_FONT_LARGE_0x30, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x31[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x10, 0xfb, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xe6, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0xd6, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x90, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x30, 0xff, 0xff, 0x4b, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0xfd, 0x4b, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x23, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x31 = { image_data_FONT_LARGE_0x31, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x32[330] = {
    0x00, 0x00, 0x00, 0x40, 0x76, 0x56, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xb4, 0xff, 0xff, 0xff, 0xcf, 0x04, 0x00, 0x00, 
    0x00, 0xb1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 
    0x20, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0xfc, 0xff, 0x7d, 0x44, 0xe7, 0xff, 0xff, 0x0c, 0x00, 
    0x00, 0xf2, 0x6f, 0x00, 0x00, 0x00, 0xfd, 0xff, 0x1f, 0x00, 
    0x00, 0x50, 0x02, 0x00, 0x00, 0x00, 0xf5, 0xff, 0x3f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff, 0x4f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf4, 0xff, 0x3f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x0f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xff, 0xff, 0x09, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xd1, 0xff, 0xff, 0x01, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x6f, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0x08, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x10, 0xfc, 0xff, 0xbf, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xd1, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xfd, 0xff, 0xbf, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xb0, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xf9, 0xff, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x40, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xc0, 0xff, 0xef, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xf3, 0xff, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xf8, 0xff, 0xaf, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 
    0x00, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 
    0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 
    0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x32 = { image_data_FONT_LARGE_0x32, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x33[330] = {
    0x00, 0x00, 0x00, 0x53, 0x77, 0x46, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x20, 0xe9, 0xff, 0xff, 0xff, 0x9f, 0x01, 0x00, 0x00, 
    0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x00, 0x00, 
    0x00, 0xf5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x01, 0x00, 
    0x00, 0xd0, 0xef, 0x48, 0x43, 0xe7, 0xff, 0xff, 0x08, 0x00, 
    0x00, 0x40, 0x05, 0x00, 0x00, 0x10, 0xfe, 0xff, 0x0e, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xff, 0x0f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0xff, 0x0f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xff, 0x0e, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xfe, 0xff, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x20, 0xe7, 0xff, 0xdf, 0x01, 0x00, 
    0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0xff, 0x2d, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0xff, 0xaf, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x76, 0xa8, 0xfd, 0xff, 0xff, 0x0b, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xfc, 0xff, 0x6f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0xff, 0xdf, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xff, 0xff, 0x02, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xff, 0xff, 0x00, 
    0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xff, 0xbf, 0x00, 
    0x00, 0xfa, 0x8c, 0x45, 0x43, 0xc7, 0xff, 0xff, 0x3f, 0x00, 
    0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 
    0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x00, 0x00, 
    0x00, 0xc8, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x02, 0x00, 0x00, 
    0x00, 0x00, 0x30, 0x75, 0x77, 0x35, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x33 = { image_data_FONT_LARGE_0x33, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x34[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x10, 0xfe, 0xff, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf9, 0xff, 0xff, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x50, 0xff, 0xef, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0xe1, 0xff, 0x4f, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0xfc, 0xff, 0x07, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x70, 0xff, 0xbf, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0xf2, 0xff, 0x1e, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0xfc, 0xff, 0x05, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x60, 0xff, 0xbf, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0xe1, 0xff, 0x2f, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0xf8, 0xff, 0x08, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x10, 0xff, 0xef, 0x01, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x90, 0xff, 0xcf, 0x88, 0x88, 0x88, 0xff, 0xff, 0x8b, 0x18, 
    0xd0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 
    0xd0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 
    0xd0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x06, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x34 = { image_data_FONT_LARGE_0x34, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x35[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0x00, 
    0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0x00, 
    0x00, 0x00, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0x00, 
    0x00, 0x00, 0xfa, 0xff, 0x9b, 0x99, 0x99, 0x99, 0x39, 0x00, 
    0x00, 0x00, 0xfb, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xfc, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xfd, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xef, 0x67, 0x25, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x20, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x02, 0x00, 0x00, 
    0x00, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6f, 0x00, 0x00, 
    0x00, 0x40, 0xdd, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x52, 0xfa, 0xff, 0xff, 0x2f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xfb, 0xff, 0xaf, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0xff, 0xff, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x03, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x02, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xff, 0xff, 0x00, 
    0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0xf5, 0xff, 0xbf, 0x00, 
    0x00, 0xf5, 0x8c, 0x45, 0x43, 0xc6, 0xff, 0xff, 0x3f, 0x00, 
    0x00, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 
    0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x00, 0x00, 
    0x00, 0xb6, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x03, 0x00, 0x00, 
    0x00, 0x00, 0x20, 0x65, 0x77, 0x36, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x35 = { image_data_FONT_LARGE_0x35, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x36[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xb7, 0xed, 0x0f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x70, 0xfe, 0xff, 0xff, 0x1f, 0x00, 
    0x00, 0x00, 0x00, 0x50, 0xfe, 0xff, 0xff, 0xff, 0x2f, 0x00, 
    0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xac, 0x29, 0x00, 
    0x00, 0x00, 0x80, 0xff, 0xff, 0xaf, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xf6, 0xff, 0xef, 0x03, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x10, 0xff, 0xff, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xf1, 0xff, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xf5, 0xff, 0x2f, 0x41, 0x55, 0x03, 0x00, 0x00, 0x00, 
    0x00, 0xfa, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x3b, 0x00, 0x00, 
    0x00, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 
    0x10, 0xff, 0xff, 0x6c, 0x12, 0x20, 0xf7, 0xff, 0xff, 0x01, 
    0x10, 0xff, 0xff, 0x05, 0x00, 0x00, 0x40, 0xff, 0xff, 0x07, 
    0x10, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x0b, 
    0x00, 0xff, 0xff, 0x06, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x0d, 
    0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x0e, 
    0x00, 0xfb, 0xff, 0x0b, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x0c, 
    0x00, 0xf7, 0xff, 0x1f, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x09, 
    0x00, 0xf1, 0xff, 0x9f, 0x00, 0x00, 0x80, 0xff, 0xff, 0x03, 
    0x00, 0xa0, 0xff, 0xff, 0x3a, 0x31, 0xf9, 0xff, 0xcf, 0x00, 
    0x00, 0x10, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2f, 0x00, 
    0x00, 0x00, 0xe2, 0xff, 0xff, 0xff, 0xff, 0xef, 0x03, 0x00, 
    0x00, 0x00, 0x10, 0xf9, 0xff, 0xff, 0xff, 0x19, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x64, 0x67, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x36 = { image_data_FONT_LARGE_0x36, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x37[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x09, 
    0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x09, 
    0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x09, 
    0x10, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xeb, 0xff, 0xff, 0x05, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff, 0x9f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0x0d, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xff, 0xaf, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x2f, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x09, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0x01, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x9f, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xf4, 0xff, 0x2f, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfb, 0xff, 0x0c, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x10, 0xff, 0xff, 0x06, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xb0, 0xff, 0xbf, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf0, 0xff, 0x6f, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf4, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf8, 0xff, 0x0e, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xfc, 0xff, 0x0a, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x20, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x40, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x60, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x37 = { image_data_FONT_LARGE_0x37, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x38[330] = {
    0x00, 0x00, 0x00, 0x10, 0x75, 0x67, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x30, 0xfb, 0xff, 0xff, 0xff, 0x18, 0x00, 0x00, 
    0x00, 0x00, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xdf, 0x02, 0x00, 
    0x00, 0x50, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1d, 0x00, 
    0x00, 0xe0, 0xff, 0xef, 0x15, 0x30, 0xfc, 0xff, 0x8f, 0x00, 
    0x00, 0xf6, 0xff, 0x2f, 0x00, 0x00, 0xb0, 0xff, 0xdf, 0x00, 
    0x00, 0xf9, 0xff, 0x0b, 0x00, 0x00, 0x40, 0xff, 0xff, 0x00, 
    0x00, 0xfb, 0xff, 0x09, 0x00, 0x00, 0x20, 0xff, 0xff, 0x01, 
    0x00, 0xfa, 0xff, 0x0c, 0x00, 0x00, 0x50, 0xff, 0xff, 0x00, 
    0x00, 0xf7, 0xff, 0x5f, 0x00, 0x00, 0xb0, 0xff, 0xaf, 0x00, 
    0x00, 0xf1, 0xff, 0xff, 0x05, 0x00, 0xf6, 0xff, 0x2f, 0x00, 
    0x00, 0x60, 0xff, 0xff, 0xcf, 0x96, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 0x4f, 0x00, 0x00, 
    0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x5e, 0x00, 0x00, 
    0x00, 0x10, 0xfd, 0xff, 0xee, 0xff, 0xff, 0xff, 0x09, 0x00, 
    0x00, 0xc0, 0xff, 0xcf, 0x01, 0xa4, 0xff, 0xff, 0x8f, 0x00, 
    0x00, 0xf6, 0xff, 0x1e, 0x00, 0x00, 0xe3, 0xff, 0xff, 0x02, 
    0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0x30, 0xff, 0xff, 0x07, 
    0x20, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 0xfb, 0xff, 0x0a, 
    0x30, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xff, 0x0b, 
    0x20, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x09, 
    0x00, 0xfe, 0xff, 0x0b, 0x00, 0x00, 0x40, 0xff, 0xff, 0x05, 
    0x00, 0xf8, 0xff, 0xcf, 0x14, 0x20, 0xf7, 0xff, 0xef, 0x00, 
    0x00, 0xd0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6f, 0x00, 
    0x00, 0x20, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 
    0x00, 0x00, 0x80, 0xfe, 0xff, 0xff, 0xff, 0x3b, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x30, 0x76, 0x67, 0x14, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x38 = { image_data_FONT_LARGE_0x38, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x39[330] = {
    0x00, 0x00, 0x00, 0x30, 0x76, 0x56, 0x01, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x60, 0xfe, 0xff, 0xff, 0xcf, 0x04, 0x00, 0x00, 
    0x00, 0x00, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x00, 0x00, 
    0x00, 0xa0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 
    0x00, 0xf5, 0xff, 0xdf, 0x14, 0x61, 0xfe, 0xff, 0x2f, 0x00, 
    0x00, 0xfd, 0xff, 0x1d, 0x00, 0x00, 0xf2, 0xff, 0x9f, 0x00, 
    0x20, 0xff, 0xff, 0x05, 0x00, 0x00, 0x90, 0xff, 0xef, 0x00, 
    0x50, 0xff, 0xff, 0x01, 0x00, 0x00, 0x30, 0xff, 0xff, 0x03, 
    0x60, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x06, 
    0x60, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x08, 
    0x40, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x09, 
    0x00, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 0xfd, 0xff, 0x09, 
    0x00, 0xfa, 0xff, 0xbf, 0x04, 0x10, 0x83, 0xff, 0xff, 0x08, 
    0x00, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 
    0x00, 0x30, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x06, 
    0x00, 0x00, 0x81, 0xfe, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x02, 
    0x00, 0x00, 0x00, 0x20, 0x54, 0x35, 0x80, 0xff, 0xef, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xbf, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xff, 0x4f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x0c, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0xff, 0xff, 0x04, 0x00, 
    0x00, 0x00, 0x00, 0x10, 0xc5, 0xff, 0xff, 0x7f, 0x00, 0x00, 
    0x00, 0x70, 0xba, 0xfd, 0xff, 0xff, 0xff, 0x08, 0x00, 0x00, 
    0x00, 0xb0, 0xff, 0xff, 0xff, 0xff, 0x5f, 0x00, 0x00, 0x00, 
    0x00, 0xb0, 0xff, 0xff, 0xff, 0x7e, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xa0, 0xde, 0xac, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x39 = { image_data_FONT_LARGE_0x39, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x2c[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x50, 0xff, 0xff, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x90, 0xff, 0xcf, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xc0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf6, 0xff, 0x0a, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xd6, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x2c = { image_data_FONT_LARGE_0x2c, 20, 33};

static const uint8_t image_data_FONT_LARGE_0x2e[330] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x94, 0x17, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x70, 0xff, 0xef, 0x01, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x09, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xb0, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x10, 0xfa, 0x6d, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const tImage FONT_LARGE_0x2e = { image_data_FONT_LARGE_0x2e, 20, 33};


static const tChar FONT_LARGE_array[] = {

  // character: '0'
  {0x30, &FONT_LARGE_0x30},
  // character: '1'
  {0x31, &FONT_LARGE_0x31},
  // character: '2'
  {0x32, &FONT_LARGE_0x32},
  // character: '3'
  {0x33, &FONT_LARGE_0x33},
  // character: '4'
  {0x34, &FONT_LARGE_0x34},
  // character: '5'
  {0x35, &FONT_LARGE_0x35},
  // character: '6'
  {0x36, &FONT_LARGE_0x36},
  // character: '7'
  {0x37, &FONT_LARGE_0x37},
  // character: '8'
  {0x38, &FONT_LARGE_0x38},
  // character: '9'
  {0x39, &FONT_LARGE_0x39},
  // character: ','
  {0x2c, &FONT_LARGE_0x2c},
  // character: '.'
  {0x2e, &FONT_LARGE_0x2e}
};


const tFont FONT_LARGE = { 12, FONT_LARGE_array };