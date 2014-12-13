#include "platform_config.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "disp6800.h"
#include "debug.h"

#ifdef DISP6800_ENABLE

#define DISP6800_DATA_DIR_IN   0
#define DISP6800_DATA_DIR_OUT  1

#define DISP6800_DISPLAY_MODE_REGULAR 0xa4
#define DISP6800_DISPLAY_MODE_ALL_ON  0xa5
#define DISP6800_DISPLAY_MODE_ALL_OFF 0xa6
#define DISP6800_DISPLAY_MODE_INVERSE 0xa7

#define DISP6800_SET_COLUMN_ADDRESS         0x15
#define DISP6800_SET_ROW_ADDRESS            0x75
#define DISP6800_SET_REMAP                  0xa0
#define DISP6800_SET_DISPLAY_OFFSET         0xa2
#define DISP6800_SET_START_LINE             0xa1
#define DISP6800_SET_DISPLAY_CLOCK          0xb3
#define DISP6800_SET_MULTIPLEX_RATIO        0xa8
#define DISP6800_SET_MASTER_CONFIG          0xad
#define DISP6800_SET_CURRENT_RANGE          0x84
#define DISP6800_SET_CONTRAST_CURRENT       0x81
#define DISP6800_SET_PHASE_LENGTH           0xb1
#define DISP6800_SET_FRAME_FREQENCY         0xb2
#define DISP6800_SET_PRECHARGE_VOLTAGE      0xbc
#define DISP6800_SET_PRECHARGE_COMPENSATION 0xb4
#define DISP6800_SET_VCOMH                  0xbe
#define DISP6800_SET_VSL                    0xbf

void disp6800_setup_gpio();
void disp6800_assert_reset();
void disp6800_deassert_reset();
void disp6800_assert_cs();
void disp6800_deassert_cs();
void disp6800_assert_en();
void disp6800_deassert_en();
void disp6800_read();
void disp6800_write();
void disp6800_command();
void disp6800_data();
uint8_t disp6800_read_status();
void disp6800_tx_command(uint8_t d);
void disp6800_set_data_port(uint8_t d);
void disp6800_set_data_dir(int dir);
void disp6800_set_display_mode(uint8_t mode);
void disp6800_set_display_onoff(uint8_t onoff);
void disp6800_set_remap_format(uint8_t remap);
void disp6800_set_display_offset(uint8_t displayOffset);
void disp6800_set_start_line(uint8_t startLine);
void disp6800_set_display_clock(uint8_t value);
void disp6800_set_multiplex_ratio(uint8_t value);
void disp6800_set_master_config(uint8_t value);
void disp6800_set_current_range(uint8_t value);
void disp6800_set_contrast_current(uint8_t value);
void disp6800_set_frame_freqency(uint8_t value);
void disp6800_set_phase_length(uint8_t value);
void disp6800_set_precharge_voltage(uint8_t value);
void disp6800_set_precharge_compensation(uint8_t a, uint8_t b);
void disp6800_set_vcomh(uint8_t value);
void disp6800_set_vsl(uint8_t value);

void disp6800_setup() {
  debug_write_line("?BEGIN disp6800_setup");
  disp6800_setup_gpio();

  disp6800_assert_reset();
  delay_ms(500);
  disp6800_deassert_reset();
  delay_ms(500);

  disp6800_set_display_onoff(DISP6800_DISPLAY_OFF);
  disp6800_set_display_clock(0x91);   // 135 Frames/second
  disp6800_set_multiplex_ratio(0x3f); // 1/64th duty
  disp6800_set_display_offset(0x4c);  // shift mapping ram counter
  disp6800_set_start_line(0x00);      // set mapping start line
  disp6800_set_master_config(0x00);   // disable embedded dc/dc converter
  disp6800_set_remap_format(0x50);
  disp6800_set_current_range(0x02);   // set full current range
  //disp6800_set_gray_scale_table();    // set pulse width for gray scale table
  disp6800_set_contrast_current(255); // set scale factor of segment output current
  disp6800_set_frame_freqency(0x51);
  disp6800_set_phase_length(0x55);    // set phase 1 as 5 clocks and phase 2 as 5 clocks
  disp6800_set_precharge_voltage(0x10);
  disp6800_set_precharge_compensation(0x20, 0x02);
  disp6800_set_vcomh(0x1c);           // set voltage high level
  disp6800_set_vsl(0x0d);             // set voltage low level
  disp6800_set_display_mode(DISP6800_DISPLAY_MODE_REGULAR);
  disp6800_set_display_onoff(DISP6800_DISPLAY_ON);
  delay_ms(200);

  disp6800_set_column_address(0x00, 0x3f);
  disp6800_set_row_address(0x00, 0x4f);

  disp6800_begin_tx_data();
  for (int i = 0; i < 5120; i++) {
    disp6800_tx_data(0);
  }

  debug_write_line("?END disp6800_setup");
}

void disp6800_setup_gpio() {
  GPIO_InitTypeDef gpioInitStructure;

  RCC_APB2PeriphClockCmd(DISP6800_RCC, ENABLE);

  GPIO_StructInit(&gpioInitStructure);

  gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  gpioInitStructure.GPIO_Pin = DISP6800_RESET_PIN;
  GPIO_Init(DISP6800_RESET, &gpioInitStructure);
  disp6800_assert_reset();

  disp6800_set_data_dir(DISP6800_DATA_DIR_IN);

  gpioInitStructure.GPIO_Pin = DISP6800_RW_PIN;
  GPIO_Init(DISP6800_RW, &gpioInitStructure);
  disp6800_read();

  gpioInitStructure.GPIO_Pin = DISP6800_EN_PIN;
  GPIO_Init(DISP6800_EN, &gpioInitStructure);
  disp6800_deassert_en();

  gpioInitStructure.GPIO_Pin = DISP6800_CS_PIN;
  GPIO_Init(DISP6800_CS, &gpioInitStructure);
  disp6800_deassert_cs();

  gpioInitStructure.GPIO_Pin = DISP6800_DC_PIN;
  GPIO_Init(DISP6800_DC, &gpioInitStructure);
  disp6800_command();
}

void disp6800_set_data_dir(int dir) {
  GPIO_InitTypeDef gpioInitStructure;

  GPIO_StructInit(&gpioInitStructure);

  if (dir == DISP6800_DATA_DIR_OUT) {
    gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  } else {
    gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  gpioInitStructure.GPIO_Pin = DISP6800_DATA0_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA1_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA2_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA3_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA4_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA5_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA6_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);

  gpioInitStructure.GPIO_Pin = DISP6800_DATA7_PIN;
  GPIO_Init(DISP6800_DATA, &gpioInitStructure);
}

void disp6800_command() {
  GPIO_ResetBits(DISP6800_DC, DISP6800_DC_PIN);
}

void disp6800_data() {
  GPIO_SetBits(DISP6800_DC, DISP6800_DC_PIN);
}

void disp6800_read() {
  disp6800_set_data_dir(DISP6800_DATA_DIR_IN);
  GPIO_SetBits(DISP6800_RW, DISP6800_RW_PIN);
}

void disp6800_write() {
  GPIO_ResetBits(DISP6800_RW, DISP6800_RW_PIN);
  disp6800_set_data_dir(DISP6800_DATA_DIR_OUT);
}

void disp6800_assert_en() {
  DISP6800_EN->BSRR = DISP6800_EN_PIN; // -> 1
}

void disp6800_deassert_en() {
  DISP6800_EN->BRR = DISP6800_EN_PIN; // -> 0
}

void disp6800_assert_reset() {
  GPIO_ResetBits(DISP6800_RESET, DISP6800_RESET_PIN);
}

void disp6800_deassert_reset() {
  GPIO_SetBits(DISP6800_RESET, DISP6800_RESET_PIN);
}

void disp6800_assert_cs() {
  DISP6800_CS->BRR = DISP6800_CS_PIN; // -> 0
}

void disp6800_deassert_cs() {
  DISP6800_CS->BSRR = DISP6800_CS_PIN; // -> 1
}

uint8_t disp6800_read_status() {
  disp6800_command();
  disp6800_read();
  disp6800_assert_cs();
  disp6800_assert_en();
  delay_us(1);
  uint8_t status = DISP6800_DATA->IDR & 0xff;
  disp6800_deassert_en();
  disp6800_deassert_cs();
  return status;
}

void disp6800_tx_command(uint8_t d) {
  disp6800_command();
  disp6800_write();
  disp6800_set_data_port(d);
  disp6800_assert_cs();
  disp6800_assert_en();
  delay_us(1);
  disp6800_deassert_en();
  disp6800_deassert_cs();
}

void disp6800_begin_tx_data() {
  disp6800_data();
  disp6800_write();
}

void disp6800_end_tx_data() {
  disp6800_deassert_cs();
}

void disp6800_tx_data(uint8_t d) {
  disp6800_set_data_port(d);
  disp6800_assert_cs();
  disp6800_assert_en();
  delay_us(1);
  disp6800_deassert_en();
  disp6800_deassert_cs();
}

void disp6800_set_data_port(uint8_t d) {
  uint32_t dd = d;
  dd = ((~dd << 16) & 0x00ff0000) | dd;
  DISP6800_DATA->BSRR = dd;
}

void disp6800_set_display_onoff(uint8_t onoff) {
  disp6800_tx_command(onoff);
}

void disp6800_set_display_mode(uint8_t mode) {
  disp6800_tx_command(mode);
}

void disp6800_set_column_address(uint8_t start, uint8_t end) {
  disp6800_tx_command(DISP6800_SET_COLUMN_ADDRESS);
  disp6800_tx_command(start);
  disp6800_tx_command(end);
}

void disp6800_set_row_address(uint8_t start, uint8_t end) {
  disp6800_tx_command(DISP6800_SET_ROW_ADDRESS);
  disp6800_tx_command(start);
  disp6800_tx_command(end);
}

void disp6800_set_remap_format(uint8_t remap) {
  disp6800_tx_command(DISP6800_SET_REMAP);
  disp6800_tx_command(remap);
}

void disp6800_set_display_offset(uint8_t displayOffset) {
  disp6800_tx_command(DISP6800_SET_DISPLAY_OFFSET);
  disp6800_tx_command(displayOffset);
}

void disp6800_set_start_line(uint8_t startLine) {
  disp6800_tx_command(DISP6800_SET_START_LINE);
  disp6800_tx_command(startLine);
}

void disp6800_set_display_clock(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_DISPLAY_CLOCK);
  disp6800_tx_command(value);
}

void disp6800_set_multiplex_ratio(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_MULTIPLEX_RATIO);
  disp6800_tx_command(value);
}

void disp6800_set_master_config(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_MASTER_CONFIG);
  disp6800_tx_command(0x02 | value);
}

void disp6800_set_current_range(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_CURRENT_RANGE | value);
}

void disp6800_set_contrast_current(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_CONTRAST_CURRENT);
  disp6800_tx_command(value);
}

void disp6800_set_frame_freqency(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_FRAME_FREQENCY);
  disp6800_tx_command(value);
}

void disp6800_set_phase_length(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_PHASE_LENGTH);
  disp6800_tx_command(value);
}

void disp6800_set_precharge_voltage(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_PRECHARGE_VOLTAGE);
  disp6800_tx_command(value);
}

void disp6800_set_precharge_compensation(uint8_t a, uint8_t b) {
  disp6800_tx_command(DISP6800_SET_PRECHARGE_COMPENSATION);
  disp6800_tx_command(b);

  if (a == 0x20) {
    disp6800_tx_command(0xb0);
    disp6800_tx_command(0x08 | a);
  }
}

void disp6800_set_vcomh(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_VCOMH);
  disp6800_tx_command(value);
}

void disp6800_set_vsl(uint8_t value) {
  disp6800_tx_command(DISP6800_SET_VSL);
  disp6800_tx_command(0x02 | value);
}

#endif
