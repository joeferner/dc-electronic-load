#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

#define MAX_LINE_LENGTH         50

#define DEBUG_LED_RCC           RCC_APB2Periph_GPIOB
#define DEBUG_LED_PORT          GPIOB
#define DEBUG_LED_PIN           GPIO_Pin_12

#define DEBUG_USART             USART1
#define DEBUG_USART_BAUD        9600
#define DEBUG_USART_RCC         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1
#define DEBUG_USART_TX          GPIOA
#define DEBUG_USART_TX_PIN      GPIO_Pin_9
#define DEBUG_USART_RX          GPIOA
#define DEBUG_USART_RX_PIN      GPIO_Pin_10
#define DEBUG_USART_RX_DMA_CH   DMA1_Channel5
#define DEBUG_USART_DR_BASE     ((uint32_t)&USART1->DR)

// SPI1 pins: SCK (GPIOA - pin 5)
// SPI1 pins: MOSI (GPIOA - pin 7)
// SPI1 pins: MISO (GPIOA - pin 6)

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
