
#ifndef _ENC28J60_H_
#define _ENC28J60_H_

#include "contiki/net/uip.h"

extern void enc28j60_spi_assert();
extern void enc28j60_spi_deassert();
extern void enc28j60_reset_assert();
extern void enc28j60_reset_deassert();
extern uint8_t enc28j60_spi_transfer(uint8_t d);

void enc28j60_setup(struct uip_eth_addr* macAddress);
void enc28j60_tick();
void enc28j60_debugDump();

#endif // _ENC28J60_H_
