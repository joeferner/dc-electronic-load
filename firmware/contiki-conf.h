#ifndef _CONTIKI_CONF_H_
#define _CONTIKI_CONF_H_

#include <stdint.h>
#include "platform_config.h"

#define CCIF
#define CLIF

#define WITH_UIP 1
#define WITH_ASCII 1

#define CLOCK_CONF_SECOND 1000

/* These names are deprecated, use C99 names. */
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int8_t s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;

typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

#ifndef BV
#define BV(x) (1<<(x))
#endif

#define UIP_CONF_IPV6_RPL 0

#define UIP_CONF_UDP     1
#define UIP_CONF_TCP     1
#define UIP_CONF_LLH_LEN 14

/* uIP configuration */
#define UIP_CONF_IP_FORWARD       1
#define UIP_CONF_TCP_SPLIT        1
#define UIP_CONF_BROADCAST        1
#define UIP_CONF_LOGGING          1
#define UIP_CONF_BUFFER_SIZE      1024
#define UIP_SPLIT_CONF_SIZE       WEBSERVER_CONF_OUTBUF_SIZE
#define RESOLV_CONF_SUPPORTS_MDNS 1
#define UIP_CONF_TCP_FORWARD      1
#define CONTIKI_CONF_DEFAULT_HOSTNAME "rs232-01"

/* Prefix for relocation sections in ELF files */
#define REL_SECT_PREFIX ".rel"

#define CC_BYTE_ALIGNED __attribute__ ((packed, aligned(1)))

#define UIP_APPCALL     tcpip_uipcall
#define UIP_UDP_APPCALL tcpip_uipcall

typedef struct tcpip_uipstate uip_tcp_appstate_t;
typedef struct tcpip_uipstate uip_udp_appstate_t;

#endif // _CONTIKI_CONF_H_
