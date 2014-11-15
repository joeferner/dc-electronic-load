
#include "recorder.h"
#include "flashsst25.h"
#include "platform_config.h"
#include "sys/process.h"
#include "sys/etimer.h"
#include "dcElectronicLoad.h"
#include "time.h"
#include "debug.h"

struct etimer recorder_etimer;
BOOL recording;
uint32_t recorder_write_addr;
uint32_t recorder_set_rate;

PROCESS(recorder_process, "Recorder");
void recorder_erase_flash();

void recorder_setup() {
  debug_write_line("?recorder_setup");
  recorder_write_addr = RECORDING_START_ADDRESS;
  recording = FALSE;
}

void recorder_start(uint32_t rate) {
  debug_write_line("?recorder_start");
  recorder_erase_flash();

  recorder_set_rate = rate;
  recorder_write_addr = RECORDING_START_ADDRESS;
  recording = TRUE;

  process_start(&recorder_process, NULL);
}

void recorder_stop() {
  debug_write_line("?recorder_stop");
  recording = FALSE;
}

BOOL recorder_is_recording() {
  return recording;
}

void recorder_erase_flash() {
  uint32_t addr;
  for (addr = RECORDING_START_ADDRESS; addr < RECORDING_END_ADDRESS; addr += (64 * 1024)) {
    flashsst25_erase_64k(addr);
  }
}

uint16_t recorder_count() {
  return (recorder_write_addr - RECORDING_START_ADDRESS) / sizeof(RecorderRecord);
}

uint32_t recorder_rate() {
  return recorder_set_rate;
}

void recorder_read(uint16_t recordNumber, RecorderRecord* record) {
  uint32_t addr = RECORDING_START_ADDRESS + (recordNumber * sizeof(RecorderRecord));
  flashsst25_readn(addr, (uint8_t*)record, sizeof(RecorderRecord));
}

PROCESS_THREAD(recorder_process, ev, data) {
  RecorderRecord record;
  PROCESS_BEGIN();

  debug_write_line("?START recorder_process");

  etimer_set(&recorder_etimer, CLOCK_SECOND * MAX(1, (recorder_set_rate / 1000)));

  while (recording) {
    PROCESS_YIELD_UNTIL(etimer_expired(&recorder_etimer));

    if (!recording) {
      debug_write_line("?recorder stopped");
      break;
    }

    if ((recorder_write_addr + sizeof(record)) >= RECORDING_END_ADDRESS) {
      debug_write_line("?recorder out of space");
      break;
    }

    debug_write_line("?record");
    record.time = time_ms();
    record.millivolts = get_millivolts();
    record.milliamps = get_milliamps();
    record.setMilliamps = get_set_milliamps();
    flashsst25_write(recorder_write_addr, (uint8_t*)&record, sizeof(record));
    recorder_write_addr += sizeof(record);

    recorder_record_irq(&record);

    etimer_reset(&recorder_etimer);
  }

  recording = FALSE;
  debug_write_line("?END recorder_process");

  PROCESS_END();
}


