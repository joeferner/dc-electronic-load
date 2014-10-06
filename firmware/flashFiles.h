
#ifndef _FLASH_FILES_H_
#define _FLASH_FILES_H_

#include <stdint.h>

struct flashFile {
  const char* filename;
  uint32_t offset;
  uint32_t size;
};

extern struct flashFile flashFiles[];

#endif
