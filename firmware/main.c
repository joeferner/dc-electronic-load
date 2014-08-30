#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include <misc.h>
#include <string.h>
#include "platform_config.h"
#include "debug.h"
#include "delay.h"
#include "time.h"
#include "ring_buffer.h"
#include "disp6800.h"
#include "gfx.h"
#include "encoder.h"
#ifdef ADC_ENABLE
#include "adc.h"
#endif
#ifdef DAC_ENABLE
#include "dac.h"
#endif
#ifdef NETWORK_ENABLED
#include "network.h"
#endif

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)

PROCESS(debug_process, "Debug");
PROCESS(gfx_update_process, "GFX Update");

void setup();
void loop();
void spi_setup();
void milli_to_string(uint16_t v, char* buffer, uint8_t display);

dma_ring_buffer g_debugUsartDmaInputRingBuffer;

#ifdef NETWORK_ENABLED
uint8_t MAC_ADDRESS[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
#endif

#define DISPLAY_MILLI     1
#define DISPLAY_1DECIMALS 2
#define DISPLAY_2DECIMALS 3
#define DISPLAY_3DECIMALS 4

uint16_t readMilliVolts;
uint16_t readCurrentMilliamps;
uint8_t readCurrentMilliampsDisplay;
uint16_t setCurrentMilliamps;

PROCINIT(
  &etimer_process
  , &debug_process
  , &gfx_update_process
#ifdef ADC_ENABLE
  , &adc_volts_process
  , &adc_current_process
#endif
#ifdef NETWORK_ENABLED
  , &tcpip_process
  , &dhcp_process
  , &telnet_process
  , &resolv_process
#endif
);

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

void setup() {
  // Configure the NVIC Preemption Priority Bits
  // 2 bit for pre-emption priority, 2 bits for subpriority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  dma_ring_buffer_init(&g_debugUsartDmaInputRingBuffer, DEBUG_USART_RX_DMA_CH, g_debugUsartRxBuffer, DEBUG_USART_RX_BUFFER_SIZE);

  debug_setup();
  debug_led_set(1);
  debug_write_line("?BEGIN setup");

  process_init();
  process_start(&etimer_process, NULL);
  process_start(&debug_process, NULL);
  process_start(&gfx_update_process, NULL);
  process_poll(&gfx_update_process);

  setCurrentMilliamps = 0;
  readCurrentMilliamps = 0;
  readMilliVolts = 0;
  readCurrentMilliampsDisplay = DISPLAY_MILLI;

  spi_setup();
  disp6800_setup();
  gfx_setup();
  encoder_setup();
#ifdef ADC_ENABLE
  adc_setup();
#endif
#ifdef DAC_ENABLE
  dac_setup();
  dac_set(0);
#endif
#ifdef NETWORK_ENABLED
  network_setup();
#endif

  time_setup();

  debug_led_set(0);
  debug_write_line("?END setup");
}

void loop() {
#ifdef NETWORK_ENABLED
  network_tick();
#endif
  process_run();
  etimer_request_poll();
  process_poll(&debug_process);

  //delay_ms(1000);
  //debug_led_set(0);
  //delay_ms(1000);
  //debug_led_set(1);
}

void assert_failed(uint8_t* file, uint32_t line) {
  debug_write("-assert_failed: file ");
  debug_write((const char*) file);
  debug_write(" on line ");
  debug_write_u32(line, 10);
  debug_write_line("");

  /* Infinite loop */
  while (1) {
  }
}

PROCESS_THREAD(gfx_update_process, ev, data) {
  char valueBuffer[20];
  char suffix[5];
  PROCESS_BEGIN();

  while (1) {
    PROCESS_YIELD();

    gfx_clear();

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
      strcpy(suffix, "A");
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

    // read read volts
    milli_to_string(readMilliVolts, valueBuffer, DISPLAY_3DECIMALS);
    gfx_draw_string(valueBuffer, &FONT_LARGE, 110, 28, GFX_ALIGN_RIGHT);
    gfx_draw_string("V", &FONT_XSMALL, 110, 42, GFX_ALIGN_LEFT);

    // TODO the next line is for the menu
    // TODO gfx_draw_string("POWER", &FONT_XSMALL, 2, 54, GFX_ALIGN_LEFT);

    gfx_redraw();
  }

  PROCESS_END();
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

#ifdef ADC_ENABLE
void adc_irq(uint8_t channel, uint16_t value) {
  if (channel == ADC_VOLTAGE_CHANNEL) {
    readMilliVolts = value;
  } else if (channel == ADC_CURRENT_CHANNEL) {
    readCurrentMilliamps = value;
  }
  process_poll(&gfx_update_process);
}
#endif

void encoder_irq(ENCODER_DIR dir) {
  int32_t newValue;
  if (dir == ENCODER_DIR_CW) {
    newValue = setCurrentMilliamps + 10;
  } else {
    newValue = setCurrentMilliamps - 10;
  }
  if (newValue < 0) {
    newValue = 0;
  } else if (newValue > MAX_SET_CURRENT) {
    newValue = MAX_SET_CURRENT;
  }
  setCurrentMilliamps = newValue;
  dac_set(setCurrentMilliamps); // TODO convert from mA to DAC value
  process_poll(&gfx_update_process);
}

PROCESS_THREAD(debug_process, ev, data) {
  char line[MAX_LINE_LENGTH];

  PROCESS_BEGIN();

  while (1) {
    PROCESS_YIELD();
    while (dma_ring_buffer_readline(&g_debugUsartDmaInputRingBuffer, line, MAX_LINE_LENGTH)) {
      if (strcmp(line, "!CONNECT\n") == 0) {
        debug_write_line("+OK");
        debug_write_line("!clear");
        debug_write_line("!set name,dc-electronic-load");
        debug_write_line("!set description,'DC Electonic Load'");
      } else if (strcmp(line, "!DISPON\n") == 0) {
        disp6800_set_display_onoff(DISP6800_DISPLAY_ON);
        debug_write_line("+OK");
      } else if (strcmp(line, "!DISPOFF\n") == 0) {
        disp6800_set_display_onoff(DISP6800_DISPLAY_OFF);
        debug_write_line("+OK");
      } else {
        debug_write("?Unknown command: ");
        debug_write_line(line);
      }
    }
  }

  PROCESS_END();
}

void spi_setup() {
  SPI_InitTypeDef spiInitStruct;
  GPIO_InitTypeDef gpioConfig;

#ifdef SPI1_ENABLE
  debug_write_line("?spi1_setup");
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

  // Configure SPI1 pins: SCK (pin 5) and MOSI (pin 7)
  gpioConfig.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpioConfig);

  // Configure SPI1 pins: MISO (pin 6)
  gpioConfig.GPIO_Pin = GPIO_Pin_6;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpioConfig);

  // init SPI
  SPI_StructInit(&spiInitStruct);
  spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiInitStruct.SPI_Mode = SPI_Mode_Master;
  spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  spiInitStruct.SPI_NSS = SPI_NSS_Soft;
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 0 (CPOL = 0, CPHA = 0)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;

  SPI_Init(SPI1, &spiInitStruct);

  SPI_Cmd(SPI1, ENABLE);
#endif

#ifdef SPI2_ENABLE
  debug_write_line("?spi2_setup");
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // Configure SPI2 pins: SCK (pin 13) and MOSI (pin 15)
  gpioConfig.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpioConfig);

  // Configure SPI2 pins: MISO (pin 14)
  gpioConfig.GPIO_Pin = GPIO_Pin_14;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &gpioConfig);

  // init SPI
  SPI_StructInit(&spiInitStruct);
  spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiInitStruct.SPI_Mode = SPI_Mode_Master;
  spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  spiInitStruct.SPI_NSS = SPI_NSS_Soft;
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 0 (CPOL = 0, CPHA = 0)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;

  SPI_Init(SPI2, &spiInitStruct);

  SPI_Cmd(SPI2, ENABLE);
#endif
}

CCIF unsigned long clock_seconds() {
  return time_ms() / 1000;
}

clock_time_t clock_time(void) {
  return time_ms();
}
