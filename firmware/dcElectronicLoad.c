
#include "platform_config.h"
#include "dcElectronicLoad.h"
#include "version.h"

PROCESS(debug_process, "Debug");

#ifdef DISP6800_ENABLE
PROCESS(gfx_update_process, "GFX Update");
#endif

#define DISPLAY_MILLI     1
#define DISPLAY_1DECIMALS 2
#define DISPLAY_2DECIMALS 3
#define DISPLAY_3DECIMALS 4

#define GFX_STATE_MEASURE 1
#define GFX_STATE_INFO    2
#define GFX_STATE_RECORD  3

#define GFX_MENU_HEIGHT   12
#define GFX_INFO_HEIGHT   72

#define MAX_SET_RATE_INDEX 4
uint32_t SET_RATES[] = { 1000, 5000, 10000, 30000, 60000 };

uint16_t readMilliVolts;
uint16_t readCurrentMilliamps;
uint8_t readCurrentMilliampsDisplay;
uint16_t setCurrentMilliamps;
uint8_t fanSetOverride;
uint16_t lastAdcValue[4];
uint8_t gfxState;
uint8_t setRateIndex;
uint32_t gfxScroll;

dma_ring_buffer g_debugUsartDmaInputRingBuffer;

void format_rate(uint32_t rateMillis, char* buffer);
void milli_to_string(uint16_t v, char* buffer, uint8_t display);
uint16_t adcVoltsToMillivolts(uint16_t value);
uint16_t adcCurrentToMillamps(uint16_t value);
uint16_t setMilliampsToDac(uint16_t value);

void gfx_draw_display_measure();
void gfx_draw_display_record();
void gfx_draw_display_info();
void gfx_draw_menu(const char* menuA, const char* menuB, const char* menuC, const char* menuD);

void dc_electronic_load_setup() {
  setCurrentMilliamps = 0;
  readCurrentMilliamps = 0;
  readMilliVolts = 0;
  setRateIndex = 0;
  readCurrentMilliampsDisplay = DISPLAY_MILLI;
  gfxState = GFX_STATE_MEASURE;

  dma_ring_buffer_init(&g_debugUsartDmaInputRingBuffer, DEBUG_USART_RX_DMA_CH, g_debugUsartRxBuffer, DEBUG_USART_RX_BUFFER_SIZE);

  debug_setup();
  debug_write_line("?BEGIN setup");

  process_init();
  process_start(&etimer_process, NULL);

  process_start(&debug_process, NULL);
#ifdef DISP6800_ENABLE
  process_start(&gfx_update_process, NULL);
  process_poll(&gfx_update_process);
#endif

  spi_setup();

#ifdef DISP6800_ENABLE
  disp6800_setup();
  gfx_setup();
#endif

#ifdef ENCODER_ENABLE
  encoder_setup();
#endif

#ifdef FLASH_ENABLE
  flashsst25_setup();
#endif

#ifdef MAC_ENABLE
  mac25aa02e48_setup();
  mac25aa02e48_read_eui48();
#endif

#ifdef ADC_ENABLE
  adc_setup();
#endif

#ifdef DAC_ENABLE
  dac_setup();
  dac_set(0);
#endif

#ifdef NETWORK_ENABLE
  network_setup(EUI48);
#endif

#ifdef FAN_ENABLE
  fan_setup();
#endif

#ifdef BUTTONS_ENABLE
  buttons_setup();
#endif

  time_setup();
  recorder_setup();

  debug_write_line("?END setup");
}

void dc_electronic_load_loop() {
#ifdef NETWORK_ENABLE
  network_tick();
#endif
  process_run();
  etimer_request_poll();
  process_poll(&debug_process);
  buttons_loop();
}

void set_current_milliamps(uint32_t value) {
  setCurrentMilliamps = value;
#ifdef DAC_ENABLE
  dac_set(setMilliampsToDac(setCurrentMilliamps));
#endif
#ifdef DISP6800_ENABLE
  process_poll(&gfx_update_process);
#endif
}

uint16_t get_millivolts() {
  return readMilliVolts;
}

uint16_t get_milliamps() {
  return readCurrentMilliamps;
}

uint16_t get_set_milliamps() {
  return setCurrentMilliamps;
}


#ifdef DISP6800_ENABLE
PROCESS_THREAD(gfx_update_process, ev, data) {
  PROCESS_BEGIN();

  while (1) {
    PROCESS_YIELD();

    gfx_clear();

    if (gfxState == GFX_STATE_MEASURE) {
      gfx_draw_display_measure();
    } else  if (gfxState == GFX_STATE_RECORD) {
      gfx_draw_display_record();
    } else {
      gfx_draw_display_info();
    }

    gfx_redraw();
  }

  PROCESS_END();
}

void gfx_draw_display_measure() {
  char valueBuffer[20];
  char suffix[5];

  // draw read current
  if (readCurrentMilliampsDisplay == DISPLAY_MILLI) {
    if (readCurrentMilliamps >= 1000) {
      readCurrentMilliampsDisplay = DISPLAY_2DECIMALS;
    }
  } else {
    if (readCurrentMilliamps < 800) {
      readCurrentMilliampsDisplay = DISPLAY_MILLI;
    }
  }
  milli_to_string(readCurrentMilliamps, valueBuffer, readCurrentMilliampsDisplay);
  if (readCurrentMilliampsDisplay == DISPLAY_MILLI) {
    strcpy(suffix, "mA");
  } else {
    strcpy(suffix, " A");
  }
  gfx_draw_string(valueBuffer, &FONT_LARGE, 110, 0, GFX_ALIGN_RIGHT);
  gfx_draw_string(suffix, &FONT_XSMALL, 110, 14, GFX_ALIGN_LEFT);

  // draw set current
  if (setCurrentMilliamps < 1000) {
    milli_to_string(setCurrentMilliamps, valueBuffer, DISPLAY_MILLI);
    strcpy(suffix, "mA");
  } else {
    milli_to_string(setCurrentMilliamps, valueBuffer, DISPLAY_2DECIMALS);
    strcpy(suffix, "A");
  }
  gfx_draw_string("SET", &FONT_XSMALL, 2, 2, GFX_ALIGN_LEFT);
  gfx_draw_string(valueBuffer, &FONT_SMALL_NUMBERS, 30, 11, GFX_ALIGN_RIGHT);
  gfx_draw_string(suffix, &FONT_XSMALL, 30, 14, GFX_ALIGN_LEFT);

  // draw read volts
  milli_to_string(readMilliVolts, valueBuffer, DISPLAY_2DECIMALS);
  gfx_draw_string(valueBuffer, &FONT_LARGE, 110, 28, GFX_ALIGN_RIGHT);
  gfx_draw_string("V", &FONT_XSMALL, 110, 42, GFX_ALIGN_LEFT);

  // draw watts
  uint16_t milliwatts = readMilliVolts * readCurrentMilliamps / 1000;
  if (milliwatts < 1000) {
    milli_to_string(milliwatts, valueBuffer, DISPLAY_MILLI);
    strcpy(suffix, "mW");
  } else {
    milli_to_string(milliwatts, valueBuffer, DISPLAY_1DECIMALS);
    strcpy(suffix, " W");
  }
  gfx_draw_string(valueBuffer, &FONT_SMALL_NUMBERS, 25, 30, GFX_ALIGN_RIGHT);
  gfx_draw_string(suffix, &FONT_XSMALL, 25, 42, GFX_ALIGN_RIGHT);

  gfx_draw_menu(recorder_is_recording() ? "*REC" : "REC", NULL, NULL, "INFO");
}

void gfx_draw_display_info() {
  char valueBuffer[20];
  char* p;
  uint8_t i;
  uip_ipaddr_t addr;

  // MAC Addr
  gfx_draw_string("MAC Addr: ", &FONT_XSMALL, 0, 0 - gfxScroll, GFX_ALIGN_LEFT);
  p = valueBuffer;
  for (i = 0; i < EUI48_LENGTH; i++) {
    if (i > 0) {
      *p++ = ':';
    }
    *p++ = TO_HEX(EUI48[i] >> 4);
    *p++ = TO_HEX(EUI48[i] >> 0);
  }
  *p = '\0';
  gfx_draw_string(valueBuffer, &FONT_XSMALL, 5, 12 - gfxScroll, GFX_ALIGN_LEFT);

  // IP Address
  gfx_draw_string("IP Addr: ", &FONT_XSMALL, 0, 24 - gfxScroll, GFX_ALIGN_LEFT);
  uip_gethostaddr(&addr);
  valueBuffer[0] = '\0';
  for (i = 0; i < 4; i++) {
    if (i > 0) {
      strcat(valueBuffer, ".");
    }
    p = valueBuffer + strlen(valueBuffer);
    uitoa(addr.u8[i], p, 10);
  }
  gfx_draw_string(valueBuffer, &FONT_XSMALL, 5, 36 - gfxScroll, GFX_ALIGN_LEFT);

  // git hash
  gfx_draw_string("GIT Hash: ", &FONT_XSMALL, 0, 48 - gfxScroll, GFX_ALIGN_LEFT);
  strncpy(valueBuffer, GIT_HASH, 7);
  valueBuffer[7] = '\0';
  gfx_draw_string(valueBuffer, &FONT_XSMALL, 5, 60 - gfxScroll, GFX_ALIGN_LEFT);

  gfx_draw_menu(NULL, NULL, NULL, "EXIT");
}

void gfx_draw_display_record() {
  char valueBuffer[20];

  // Records
  gfx_draw_string("Records: ", &FONT_XSMALL, 0, 0, GFX_ALIGN_LEFT);
  itoa(recorder_count(), valueBuffer, 10);
  gfx_draw_string(valueBuffer, &FONT_XSMALL, 5, 12, GFX_ALIGN_LEFT);

  // Rate
  gfx_draw_string("Rate: ", &FONT_XSMALL, 0, 24, GFX_ALIGN_LEFT);
  format_rate(recorder_is_recording() ? recorder_rate() : SET_RATES[CLAMP(setRateIndex, 0, MAX_SET_RATE_INDEX)], valueBuffer);
  gfx_draw_string(valueBuffer, &FONT_XSMALL, 5, 36, GFX_ALIGN_LEFT);

  gfx_draw_menu(recorder_is_recording() ? "STOP" : "START", NULL, NULL, "EXIT");
}

void gfx_draw_menu(const char* menuA, const char* menuB, const char* menuC, const char* menuD) {
  gfx_fill_rect(0, GFX_HEIGHT - GFX_MENU_HEIGHT, GFX_WIDTH, GFX_MENU_HEIGHT, GFX_COLOR_BLACK);

  if (menuA) {
    gfx_draw_string(menuA, &FONT_XSMALL, 15, 54, GFX_ALIGN_CENTER);
  }
  if (menuB) {
    gfx_draw_string(menuB, &FONT_XSMALL, 48, 54, GFX_ALIGN_CENTER);
  }
  if (menuC) {
    gfx_draw_string(menuC, &FONT_XSMALL, 77, 54, GFX_ALIGN_CENTER);
  }
  if (menuD) {
    gfx_draw_string(menuD, &FONT_XSMALL, 110, 54, GFX_ALIGN_CENTER);
  }
}

#endif

#ifdef BUTTONS_ENABLE
void buttons_irq(uint8_t buttons) {
  if (gfxState == GFX_STATE_MEASURE) {
    if (buttons & BUTTON_A) { // record screen
      gfxState = GFX_STATE_RECORD;
    } else if (buttons & BUTTON_D) { // info
      gfxScroll = 0;
      gfxState = GFX_STATE_INFO;
    }
  } else if (gfxState == GFX_STATE_INFO) {
    if (buttons & BUTTON_D) { // exit
      gfxState = GFX_STATE_MEASURE;
    }
  } else if (gfxState == GFX_STATE_RECORD) {
    if (buttons & BUTTON_A) { // start/stop
      if (recorder_is_recording()) {
        recorder_stop();
      } else {
        recorder_start(1000);
      }
    } else if (buttons & BUTTON_D) { // exit
      gfxState = GFX_STATE_MEASURE;
    }
  }

#ifdef DISP6800_ENABLE
  process_poll(&gfx_update_process);
#endif
}
#endif

void recorder_record_irq(RecorderRecord* record) {
#ifdef DISP6800_ENABLE
  if (gfxState == GFX_STATE_RECORD) {
    process_poll(&gfx_update_process);
  }
#endif
}

void format_rate(uint32_t rateMillis, char* buffer) {
  if (rateMillis < 1000) {
    itoa(rateMillis, buffer, 10);
    strcat(buffer, "ms");
  } else if (rateMillis < 60000) {
    itoa(rateMillis / 1000, buffer, 10);
    strcat(buffer, "s");
  } else {
    itoa(rateMillis / 60000, buffer, 10);
    strcat(buffer, "m");
  }
}

void milli_to_string(uint16_t v, char* buffer, uint8_t display) {
  char tempBuffer[20];
  int decimals;

  if (display == DISPLAY_MILLI) {
    uitoa(v, tempBuffer, 10);
    addCommas(tempBuffer, buffer);
  } else {
    if (display == DISPLAY_3DECIMALS) {
      decimals = 3;
    } else if (display == DISPLAY_2DECIMALS) {
      decimals = 2;
    } else if (display == DISPLAY_1DECIMALS) {
      decimals = 1;
    }
    uitoa(v % 1000, buffer, 10);
    padLeft(buffer, tempBuffer, decimals, '0');
    tempBuffer[decimals] = '\0';
    uitoa(v / 1000, buffer, 10);
    strcat(buffer, ".");
    strcat(buffer, tempBuffer);
  }
}

uint16_t adcVoltsToMillivolts(uint16_t value) {
  return value * 6.0318341;
}

uint16_t adcCurrentToMillamps(uint16_t value) {
  return value * 1.2521904632;
}

uint16_t setMilliampsToDac(uint16_t value) {
  return value / 1.2427047619;
}

#ifdef ADC_ENABLE
void adc_irq(uint8_t channel, uint16_t value) {
  lastAdcValue[channel] = value;

  if (channel == ADC_VOLTAGE_CHANNEL) {
    readMilliVolts = adcVoltsToMillivolts(value);
  } else if (channel == ADC_CURRENT_CHANNEL) {
    readCurrentMilliamps = adcCurrentToMillamps(value);
  } else if (channel == ADC_TEMP1_CHANNEL || channel == ADC_TEMP2_CHANNEL) {
    if (lastAdcValue[ADC_TEMP1_CHANNEL] < 2500 || lastAdcValue[ADC_TEMP2_CHANNEL] < 2500) {
      fan_set(100);
    }
    if (lastAdcValue[ADC_TEMP1_CHANNEL] > 2600 && lastAdcValue[ADC_TEMP2_CHANNEL] > 2600) {
      fan_set(0);
    }
  }
#ifdef DISP6800_ENABLE
  process_poll(&gfx_update_process);
#endif
}
#endif

#ifdef ENCODER_ENABLE
void encoder_irq(ENCODER_DIR dir) {
  int32_t newValue;
  if (gfxState == GFX_STATE_MEASURE) {
    if (dir == ENCODER_DIR_CW) {
      newValue = setCurrentMilliamps + 10;
    } else {
      newValue = setCurrentMilliamps - 10;
    }
    newValue = CLAMP(newValue, 0, MAX_SET_CURRENT);
    set_current_milliamps(newValue);
  } else if (gfxState == GFX_STATE_RECORD) {
    if (!recorder_is_recording()) {
      if (dir == ENCODER_DIR_CW) {
        newValue = setRateIndex + 1;
      } else {
        newValue = setRateIndex - 1;
      }
      newValue = CLAMP(newValue, 0, MAX_SET_RATE_INDEX);
      setRateIndex = newValue;
    }
#ifdef DISP6800_ENABLE
    process_poll(&gfx_update_process);
#endif
  } else if (gfxState == GFX_STATE_INFO) {
    if (dir == ENCODER_DIR_CW) {
      newValue = gfxScroll + 2;
    } else {
      newValue = gfxScroll - 2;
    }
    gfxScroll = CLAMP(newValue, 0, GFX_INFO_HEIGHT - (GFX_HEIGHT - GFX_MENU_HEIGHT));
#ifdef DISP6800_ENABLE
    process_poll(&gfx_update_process);
#endif
  }
}

void encoder_button_irq() {
  if (gfxState == GFX_STATE_MEASURE) {
    set_current_milliamps(0);
  } else if (gfxState == GFX_STATE_RECORD) {
    if (!recorder_is_recording()) {
      setRateIndex = 0;
    }
#ifdef DISP6800_ENABLE
    process_poll(&gfx_update_process);
#endif
  }
}

#endif

PROCESS_THREAD(debug_process, ev, data) {
  char line[MAX_LINE_LENGTH];
  uint16_t read;
  uint8_t b;
  uint32_t flashAddress = 0;
  uint32_t flashCount = 0;
  uint32_t t = 0;

  PROCESS_BEGIN();

  while (1) {
    PROCESS_YIELD();

    while (dma_ring_buffer_readline(&g_debugUsartDmaInputRingBuffer, line, MAX_LINE_LENGTH)) {
      if (strcmp(line, "!CONNECT\n") == 0) {
        debug_write_line("+OK");
        debug_write_line("!clear");
        debug_write_line("!set name,dc-electronic-load");
        debug_write_line("!set description,'DC Electonic Load'");
      }

#ifdef ADC_ENABLE
      else if (strcmp(line, "!ADCLAST\n") == 0) {
        debug_write("+OK ");
        debug_write_u16(lastAdcValue[0], 10);
        debug_write(",");
        debug_write_u16(lastAdcValue[1], 10);
        debug_write(",");
        debug_write_u16(lastAdcValue[2], 10);
        debug_write(",");
        debug_write_u16(lastAdcValue[3], 10);
        debug_write(",");
        debug_write_u16(setCurrentMilliamps, 10);
        debug_write_line("");
      } else if (strcmp(line, "!ADCRAW\n") == 0) {
        debug_write("+OK ");
        debug_write_u16(adc_sample(ADC_CH0_SINGLE), 10);
        debug_write(",");
        debug_write_u16(adc_sample(ADC_CH1_SINGLE), 10);
        debug_write(",");
        debug_write_u16(adc_sample(ADC_CH2_SINGLE), 10);
        debug_write(",");
        debug_write_u16(adc_sample(ADC_CH3_SINGLE), 10);
        debug_write_line("");
      }
#endif

#ifdef FAN_ENABLE
      else if (strncmp(line, "!FANSET ", 8) == 0) {
        uint8_t fanSetOverride = atoi(line + 8);
        fan_set(fanSetOverride);
        debug_write_line("+OK");
      } else if (strcmp(line, "!FANGET\n") == 0) {
        debug_write("+OK ");
        debug_write_u8(fan_get(), 10);
        debug_write_line("");
      }
#endif

#ifdef DISP6800_ENABLE
      else if (strcmp(line, "!DISPON\n") == 0) {
        disp6800_set_display_onoff(DISP6800_DISPLAY_ON);
        debug_write_line("+OK");
      } else if (strcmp(line, "!DISPOFF\n") == 0) {
        disp6800_set_display_onoff(DISP6800_DISPLAY_OFF);
        debug_write_line("+OK");
      }
#endif

#ifdef FLASH_ENABLE
      else if (strcmp(line, "!FLASHCLEAR\n") == 0) {
        flashsst25_erase_chip();
        debug_write_line("+OK");
      } else if (strncmp(line, "!FLASHWRITE ", 12) == 0) {
        flashAddress = atoi(line + 12);
        flashCount = FLASH_BLOCK_SIZE;

        debug_write_line("+READY");
        t = time_ms();
        while (1) {
          if (flashCount <= 0) {
            debug_write("+OK ");
            debug_write_u32(flashAddress, 10);
            debug_write_line("");
            break;
          }
          if ((time_ms() - t) > 5000) {
            debug_write_line("-FAIL");
            break;
          }
          if ((read = dma_ring_buffer_read(&g_debugUsartDmaInputRingBuffer, (uint8_t*)line, MIN(flashCount, sizeof(line)))) <= 0) {
            continue;
          }
          flashsst25_write(flashAddress, (uint8_t*)line, read);
          flashAddress += read;
          flashCount -= read;
          t = time_ms();
        }
      } else if (strncmp(line, "!FLASHREAD ", 11) == 0) {
        flashAddress = atoi(line + 11);
        flashCount = FLASH_BLOCK_SIZE;
        debug_write_line("+OK");
        flashsst25_read_begin(flashAddress);
        while (flashCount > 0) {
          b = flashsst25_read();
          debug_write_bytes(&b, 1);
          flashCount--;
        }
        flashsst25_read_end();
      }
#endif

      else {
        debug_write("?Unknown command: ");
        debug_write_line(line);
      }
    }
  }

  PROCESS_END();
}
