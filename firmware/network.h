
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdint.h>
#include "enc28j60.h"
#include "contiki/net/ip/resolv.h"
#include "httpd.h"

PROCESS_NAME(dhcp_process);
PROCESS_NAME(telnet_process);

void network_setup(uint8_t* macAddress);
void network_tick();

#endif // _NETWORK_H_
