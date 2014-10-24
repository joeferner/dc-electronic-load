#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

#include <misc.h>
#include <stm32f10x_rcc.h>

#define HTTPD_CONNS                8
#define WEBSERVER_CONF_CFS_PATHLEN 20
#define WEBSERVER_CONF_OUTBUF_SIZE 600
#define DEBUG_USART_RX_BUFFER_SIZE 600

#define SPI1_ENABLE
#define SPI2_ENABLE
#define NETWORK_ENABLE
#define ADC_ENABLE
#define DAC_ENABLE
#define DISP6800_ENABLE
#define FAN_ENABLE
#define ENCODER_ENABLE
#define FLASH_ENABLE
#define MAC_ENABLE

// SPI1 pins: SCK (GPIOA5)
// SPI1 pins: MISO (GPIOA6)
// SPI1 pins: MOSI (GPIOA7)

// SPI2 pins: SCK (GPIOB13)
// SPI2 pins: MISO (GPIOB14)
// SPI2 pins: MOSI (GPIOB15)

#define MAX_SET_CURRENT         5000
#define MAX_LINE_LENGTH         50

#define DEBUG_USART             USART1
#define DEBUG_USART_BAUD        19200
#define DEBUG_USART_RCC         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1
#define DEBUG_USART_TX          GPIOA
#define DEBUG_USART_TX_PIN      GPIO_Pin_9
#define DEBUG_USART_RX          GPIOA
#define DEBUG_USART_RX_PIN      GPIO_Pin_10
#define DEBUG_USART_RX_DMA_CH   DMA1_Channel5
#define DEBUG_USART_DR_BASE     ((uint32_t)&USART1->DR)

#ifdef DISP6800_ENABLE
#define DISP6800_RCC            RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD

#define DISP6800_DATA           GPIOB
#define DISP6800_DATA0_PIN      GPIO_Pin_0
#define DISP6800_DATA1_PIN      GPIO_Pin_1
#define DISP6800_DATA2_PIN      GPIO_Pin_2
#define DISP6800_DATA3_PIN      GPIO_Pin_3
#define DISP6800_DATA4_PIN      GPIO_Pin_4
#define DISP6800_DATA5_PIN      GPIO_Pin_5
#define DISP6800_DATA6_PIN      GPIO_Pin_6
#define DISP6800_DATA7_PIN      GPIO_Pin_7
#define DISP6800_RW             GPIOC
#define DISP6800_RW_PIN         GPIO_Pin_10
#define DISP6800_EN             GPIOA
#define DISP6800_EN_PIN         GPIO_Pin_15
#define DISP6800_RESET          GPIOC
#define DISP6800_RESET_PIN      GPIO_Pin_11
#define DISP6800_CS             GPIOC
#define DISP6800_CS_PIN         GPIO_Pin_12
#define DISP6800_DC             GPIOD
#define DISP6800_DC_PIN         GPIO_Pin_2
#endif

#ifdef ENCODER_ENABLE
#define ENCODER_CH_A_EXTI       EXTI_Line0
#define ENCODER_CH_A_EXTI_PORT  GPIO_PortSourceGPIOC
#define ENCODER_CH_A_EXTI_PIN   GPIO_PinSource0
#define ENCODER_CH_A_RCC        RCC_APB2Periph_GPIOC
#define ENCODER_CH_A_PORT       GPIOC
#define ENCODER_CH_A_PIN        GPIO_Pin_0

#define ENCODER_CH_B_EXTI       EXTI_Line1
#define ENCODER_CH_B_EXTI_PORT  GPIO_PortSourceGPIOC
#define ENCODER_CH_B_EXTI_PIN   GPIO_PinSource1
#define ENCODER_CH_B_RCC        RCC_APB2Periph_GPIOC
#define ENCODER_CH_B_PORT       GPIOC
#define ENCODER_CH_B_PIN        GPIO_Pin_1

#define ENCODER_BUTTON_EXTI       EXTI_Line2
#define ENCODER_BUTTON_EXTI_PORT  GPIO_PortSourceGPIOC
#define ENCODER_BUTTON_EXTI_PIN   GPIO_PinSource2
#define ENCODER_BUTTON_RCC        RCC_APB2Periph_GPIOC
#define ENCODER_BUTTON_PORT       GPIOC
#define ENCODER_BUTTON_PIN        GPIO_Pin_2
#endif

#ifdef ADC_ENABLE
#ifndef SPI2_ENABLE
  #error "SPI2 is required for adc"
#endif
#define ADC_CS_RCC              RCC_APB2Periph_GPIOC
#define ADC_CS_PORT             GPIOC
#define ADC_CS_PIN              GPIO_Pin_6
#define ADC_SPI                 SPI2

#define ADC_VOLTAGE_CHANNEL     0
#define ADC_CURRENT_CHANNEL     1
#define ADC_TEMP1_CHANNEL       2
#define ADC_TEMP2_CHANNEL       3
#endif

#ifdef DAC_ENABLE
#ifndef SPI2_ENABLE
  #error "SPI2 is required for dac"
#endif
#define DAC_CS_RCC              RCC_APB2Periph_GPIOC
#define DAC_CS_PORT             GPIOC
#define DAC_CS_PIN              GPIO_Pin_7
#define DAC_SPI                 SPI2
#endif

#ifdef FLASH_ENABLE
#ifndef SPI2_ENABLE
  #error "SPI2 is required for flash"
#endif
#define FLASH_CS_RCC            RCC_APB2Periph_GPIOC
#define FLASH_CS_PORT           GPIOC
#define FLASH_CS_PIN            GPIO_Pin_8
#define FLASH_SPI               SPI2
#endif

#ifdef MAC_ENABLE
#ifndef SPI2_ENABLE
  #error "SPI2 is required for mac"
#endif
#define MAC_CS_RCC              RCC_APB2Periph_GPIOC
#define MAC_CS_PORT             GPIOC
#define MAC_CS_PIN              GPIO_Pin_9
#define MAC_SPI                 SPI2
#endif

#ifdef FAN_ENABLE
#define FAN_RCC2            RCC_APB2Periph_GPIOA
#define FAN_PORT            GPIOA
#define FAN_PIN             GPIO_Pin_8
#endif

#ifdef NETWORK_ENABLE
#ifndef SPI1_ENABLE
  #error "SPI1 is required for network"
#endif
#define ENC28J60_CS_RCC         RCC_APB2Periph_GPIOC
#define ENC28J60_CS_PORT        GPIOC
#define ENC28J60_CS_PIN         GPIO_Pin_4

#define ENC28J60_RESET_RCC      RCC_APB2Periph_GPIOC
#define ENC28J60_RESET_PORT     GPIOC
#define ENC28J60_RESET_PIN      GPIO_Pin_5

// port 23 is the traditional telnet port
#define TELNET_PORT             23
#endif

#endif // PLATFORM_CONFIG_H_INCLUDED
