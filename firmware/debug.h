#ifndef DEBUG_H
#define	DEBUG_H

#include <stdint.h>
#include "contiki/net/ipv4/uip_arp.h"
#include "stringUtils.h"
#include <stm32f10x_dma.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define DEBUG_USART_RX_BUFFER_SIZE 100

extern uint8_t g_debugUsartRxBuffer[DEBUG_USART_RX_BUFFER_SIZE];

void debug_setup();
void debug_led_set(int v);
void debug_write_line(const char* str);
void debug_write_bytes(const uint8_t* data, uint16_t len);
void debug_write(const char* str);
void debug_write_ch(char ch);
void debug_write_u8(uint8_t val, uint8_t base);
void debug_write_u8_array(uint8_t* p, int len);
void debug_write_u16(uint16_t val, uint8_t base);
void debug_write_u32(uint32_t val, uint8_t base);
void debug_write_i32(int32_t val, uint8_t base);
void debug_write_uip_eth_addr(const struct uip_eth_addr* addr);
void debug_write_uip_ip_addr(const uip_ipaddr_t* addr);

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

