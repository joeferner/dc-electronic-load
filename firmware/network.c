
#include "platform_config.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <string.h>
#include "network.h"
#include "contiki/net/ipv4/uip_arp.h"
#include "contiki/net/ip/dhcpc.h"
#include "contiki-conf.h"
#include "debug.h"
#include "ring_buffer.h"
#include "flashFiles.h"
#include "flashsst25.h"
#include "util.h"

#ifdef NETWORK_ENABLE

PROCESS(dhcp_process, "DHCP");
PROCESS(telnet_process, "Telnet");

uint8_t _network_uip_headerLength = 0;
uint8_t _network_request_dhcp = 0;

void network_setup(uint8_t* macAddress) {
  debug_write_line("?BEGIN network_setup");

  uip_ipaddr_t ipaddr;
  uip_ipaddr_t gatewayAddr;
  uip_ipaddr_t netmaskAddr;
  GPIO_InitTypeDef gpioConfig;

  enc28j60_reset_deassert();
  RCC_APB2PeriphClockCmd(ENC28J60_RESET_RCC, ENABLE);
  gpioConfig.GPIO_Pin = ENC28J60_RESET_PIN;
  gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ENC28J60_RESET_PORT, &gpioConfig);
  enc28j60_reset_deassert();

  enc28j60_spi_deassert(); // set pin high before initializing as output pin to not false trigger CS
  RCC_APB2PeriphClockCmd(ENC28J60_CS_RCC, ENABLE);
  gpioConfig.GPIO_Pin = ENC28J60_CS_PIN;
  gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ENC28J60_CS_PORT, &gpioConfig);
  enc28j60_spi_deassert();

  uip_lladdr.addr[0] = macAddress[0];
  uip_lladdr.addr[1] = macAddress[1];
  uip_lladdr.addr[2] = macAddress[2];
  uip_lladdr.addr[3] = macAddress[3];
  uip_lladdr.addr[4] = macAddress[4];
  uip_lladdr.addr[5] = macAddress[5];

  uip_ipaddr(&ipaddr, 0, 0, 0, 0);
  uip_sethostaddr(&ipaddr);

  uip_ipaddr(&gatewayAddr, 0, 0, 0, 0);
  uip_setdraddr(&gatewayAddr);

  uip_ipaddr(&netmaskAddr, 0, 0, 0, 0);
  uip_setnetmask(&netmaskAddr);

  enc28j60_setup(&uip_lladdr);

  uip_init();
  uip_arp_init();

  debug_write_line("?Start DHCP Process");
  _network_request_dhcp = 1;
  process_start(&dhcp_process, NULL);

  debug_write_line("?Start Telnet Process");
  process_start(&telnet_process, NULL);

  debug_write_line("?Start HTTPD Process");
  process_start(&httpd_process, NULL);

  debug_write_line("?END network_setup");
}

void network_tick() {
  enc28j60_tick();
  process_poll(&telnet_process);
}

void enc28j60_reset_assert() {
  GPIO_ResetBits(ENC28J60_RESET_PORT, ENC28J60_RESET_PIN);
}

void enc28j60_reset_deassert() {
  GPIO_SetBits(ENC28J60_RESET_PORT, ENC28J60_RESET_PIN);
}

void enc28j60_spi_assert() {
  GPIO_ResetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

void enc28j60_spi_deassert() {
  GPIO_SetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

uint8_t enc28j60_spi_transfer(uint8_t d) {
  SPI_I2S_SendData(SPI1, d);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
  return SPI_I2S_ReceiveData(SPI1);
}

PROCESS_THREAD(telnet_process, ev, data) {
  int i;
  uint8_t* p;

  PROCESS_BEGIN();
  tcp_listen(UIP_HTONS(TELNET_PORT));

  while (1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_EXIT) {
      process_exit(&dhcp_process);
      LOADER_UNLOAD();
    } else if (ev == PROCESS_EVENT_POLL) {
      for (i = 0; i < UIP_CONNS; i++) {
        if (uip_conn_active(i)) {
          uip_poll_conn(&uip_conns[i]);
        }
      }
    } else if (ev == tcpip_event) {
      if (uip_connected()) {
        tcp_markconn(uip_conn, NULL);
      } else if (uip_newdata()) {
        p = uip_appdata;
        debug_write("!netdata:");
        for (i = 0; i < uip_len; i++) {
          debug_write_ch(p[i]);
        }
        debug_write_line("");
      }
    }
  }

  PROCESS_END();
}


PROCESS_THREAD(dhcp_process, ev, data) {
  PROCESS_BEGIN();
  dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));

  while (1) {
    PROCESS_WAIT_EVENT();
    if (_network_request_dhcp) {
      _network_request_dhcp = 0;
      dhcpc_request();
    } else if (ev == PROCESS_EVENT_EXIT) {
      process_exit(&dhcp_process);
      LOADER_UNLOAD();
    } else {
      dhcpc_appcall(ev, data);
    }
  }

  PROCESS_END();
}

void dhcpc_configured(const struct dhcpc_state* s) {
  debug_write_line("?dhcpc_configured");
  debug_write("?ipaddr");
  debug_write_uip_ip_addr(&s->ipaddr);
  debug_write_line("");
  debug_write("?default_router");
  debug_write_uip_ip_addr(&s->default_router);
  debug_write_line("");
  debug_write("?netmask");
  debug_write_uip_ip_addr(&s->netmask);
  debug_write_line("");

  uip_sethostaddr(&s->ipaddr);
  uip_setdraddr(&s->default_router);
  uip_setnetmask(&s->netmask);

  process_start(&resolv_process, NULL);
}

void dhcpc_unconfigured(const struct dhcpc_state* s) {
  debug_write_line("?dhcpc_unconfigured");
}

PT_THREAD(serve_flash_file(struct httpd_state* s)) {
  uint32_t readlen;

  PSOCK_BEGIN(&s->sout);

  while (s->file_pos < s->file->size) {
    readlen = MIN(HTTPD_OUTBUF_SIZE, s->file->size - s->file_pos);
    flashsst25_readn(s->file->offset + s->file_pos, s->outbuf, readlen);
    s->file_pos += readlen;
    s->outbuf_pos = readlen;
    PSOCK_SEND(&s->sout, s->outbuf, s->outbuf_pos);
  }

  PSOCK_END(&s->sout);
}

PT_THREAD(serve_amps_set(struct httpd_state* s)) {
  PSOCK_BEGIN(&s->sout);

  strcpy((char*)s->outbuf, "OK");
  s->outbuf_pos = strlen((char*)s->outbuf);

  PSOCK_END(&s->sout);
}

PT_THREAD(serve_web_socket(struct httpd_state* s)) {
  PSOCK_BEGIN(&s->sout);

  strcpy((char*)s->outbuf, "OK");
  s->outbuf_pos = strlen((char*)s->outbuf);

  PSOCK_END(&s->sout);
}

struct flashFile* httpd_get_file(const char* filename) {
  for (uint8_t i = 0; ; i++) {
    struct flashFile* f = &flashFiles[i];
    if (f->filename == NULL) {
      break;
    }
    if (strcmp(filename, f->filename) == 0) {
      return f;
    }
  }
  return NULL;
}

#endif
