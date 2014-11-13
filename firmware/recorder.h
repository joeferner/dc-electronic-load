
#ifndef _RECORDER_H_
#define _RECORDER_H_

#include "util.h"

typedef struct {
  uint32_t time;
  uint16_t millivolts;
  uint16_t milliamps;
  uint16_t setMilliamps;
} RecorderRecord;

void recorder_setup();
void recorder_start();
void recorder_stop();
BOOL recorder_is_recording();
uint16_t recorder_count();
void recorder_read(uint16_t recordNumber, RecorderRecord* record);

#endif
