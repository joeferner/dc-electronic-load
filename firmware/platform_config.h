#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

#include <misc.h>
#include <stm32f10x_rcc.h>

#define MAX_SET_CURRENT         5000
#define MAX_LINE_LENGTH         50

#define DEBUG_LED_RCC           RCC_APB2Periph_GPIOB
#define DEBUG_LED_PORT          GPIOB
#define DEBUG_LED_PIN           GPIO_Pin_0

#define DEBUG_USART             USART1
#define DEBUG_USART_BAUD        9600
#define DEBUG_USART_RCC         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1
#define DEBUG_USART_TX          GPIOA
#define DEBUG_USART_TX_PIN      GPIO_Pin_9
#define DEBUG_USART_RX          GPIOA
#define DEBUG_USART_RX_PIN      GPIO_Pin_10
#define DEBUG_USART_RX_DMA_CH   DMA1_Channel5
#define DEBUG_USART_DR_BASE     ((uint32_t)&USART1->DR)

#define DISP6800_RCC            RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB

#define DISP6800_DATA           GPIOA
#define DISP6800_DATA0_PIN      GPIO_Pin_0
#define DISP6800_DATA1_PIN      GPIO_Pin_1
#define DISP6800_DATA2_PIN      GPIO_Pin_2
#define DISP6800_DATA3_PIN      GPIO_Pin_3
#define DISP6800_DATA4_PIN      GPIO_Pin_4
#define DISP6800_DATA5_PIN      GPIO_Pin_5
#define DISP6800_DATA6_PIN      GPIO_Pin_6
#define DISP6800_DATA7_PIN      GPIO_Pin_7
#define DISP6800_RW             GPIOB
#define DISP6800_RW_PIN         GPIO_Pin_8
#define DISP6800_EN             GPIOB
#define DISP6800_EN_PIN         GPIO_Pin_9
#define DISP6800_RESET          GPIOB
#define DISP6800_RESET_PIN      GPIO_Pin_10
#define DISP6800_CS             GPIOB
#define DISP6800_CS_PIN         GPIO_Pin_11
#define DISP6800_DC             GPIOB
#define DISP6800_DC_PIN         GPIO_Pin_12

#define ENCODER_CH_A_EXTI       EXTI_Line6
#define ENCODER_CH_A_EXTI_PORT  GPIO_PortSourceGPIOB
#define ENCODER_CH_A_EXTI_PIN   GPIO_PinSource6
#define ENCODER_CH_A_RCC        RCC_APB2Periph_GPIOB
#define ENCODER_CH_A_PORT       GPIOB
#define ENCODER_CH_A_PIN        GPIO_Pin_6
#define ENCODER_CH_B_EXTI       EXTI_Line7
#define ENCODER_CH_B_EXTI_PORT  GPIO_PortSourceGPIOB
#define ENCODER_CH_B_EXTI_PIN   GPIO_PinSource7
#define ENCODER_CH_B_RCC        RCC_APB2Periph_GPIOB
#define ENCODER_CH_B_PORT       GPIOB
#define ENCODER_CH_B_PIN        GPIO_Pin_7

#define ADC_ENABLE
#define ADC_CS_RCC              RCC_APB2Periph_GPIOB
#define ADC_CS_PORT             GPIOB
#define ADC_CS_PIN              GPIO_Pin_1
#define ADC_SPI                 SPI2

#define ADC_VOLTAGE_CHANNEL     0
#define ADC_CURRENT_CHANNEL     1
#define ADC_TEMP1_CHANNEL       2
#define ADC_TEMP2_CHANNEL       3

#define DAC_ENABLE
#define DAC_CS_RCC              RCC_APB2Periph_GPIOB
#define DAC_CS_PORT             GPIOB
#define DAC_CS_PIN              GPIO_Pin_2
#define DAC_SPI                 SPI2

#define FAN_ENABLE
#define FAN_PWM_RCC1            RCC_APB1Periph_TIM3
#define FAN_PWM_RCC2            RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define FAN_PWM_PORT            GPIOB
#define FAN_PWM_PIN             GPIO_Pin_5
#define FAN_PWM_TIMER           TIM3
#define FAN_PWM_TIMER_REMAP             GPIO_PartialRemap_TIM3
#define FAN_PWM_TIMER_CH_INIT           TIM_OC2Init
#define FAN_PWM_TIMER_CH_PRELOAD_CONFIG TIM_OC2PreloadConfig

// #define SPI1_ENABLE
// SPI1 pins: SCK (GPIOA - pin 5)
// SPI1 pins: MISO (GPIOA - pin 6)
// SPI1 pins: MOSI (GPIOA - pin 7)

#define SPI2_ENABLE
// SPI2 pins: SCK (GPIOA - pin 13)
// SPI2 pins: MISO (GPIOA - pin 14)
// SPI2 pins: MOSI (GPIOA - pin 15)

#undef NETWORK_ENABLED

#ifdef NETWORK_ENABLED
#define ENC28J60_CS_RCC         RCC_APB2Periph_GPIOB
#define ENC28J60_CS_PORT        GPIOB
#define ENC28J60_CS_PIN         GPIO_Pin_0

#define ENC28J60_RESET_RCC      RCC_APB2Periph_GPIOB
#define ENC28J60_RESET_PORT     GPIOB
#define ENC28J60_RESET_PIN      GPIO_Pin_1

// port 23 is the traditional telnet port
#define TELNET_PORT             23
#endif

#endif // PLATFORM_CONFIG_H_INCLUDED
