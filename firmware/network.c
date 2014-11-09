
#include "platform_config.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <string.h>
#include <stdlib.h>
#include "network.h"
#include "contiki/net/ipv4/uip_arp.h"
#include "contiki/net/ip/dhcpc.h"
#include "contiki/net/ipv4/uip-fw.h"
#include "contiki/net/ipv4/uip-fw-drv.h"
#include "net/ip/tcpip.h"
#include "contiki-conf.h"
#include "debug.h"
#include "ring_buffer.h"
#include "flashFiles.h"
#include "flashsst25.h"
#include "util.h"
#include "dcElectronicLoad.h"

#ifdef NETWORK_ENABLE

#define WS_FIN          0x80
#define WS_OPCODE_TEXT  0x01
#define WS_MASK         0x80

static const char http_header_200[] = "HTTP/1.0 200 OK\r\nCache-Control: public, max-age=864000\r\nConnection: close\r\n";
static const char http_200_ok[] = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: 2\r\n\r\nOK";
static const char http_400_fail[] = "HTTP/1.0 200 BAD\r\nConnection: close\r\nContent-Length: 4\r\n\r\nFAIL";
static const char http_header_101_ws_upgrade[] = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n";

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
  uip_fw_init();
  uip_arp_init();

  debug_write_line("?Start uip-fw");
  process_start(&uip_fw_process, NULL);
  
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

PT_THREAD(serve_flash_file(process_event_t ev, struct httpd_state* s)) {
  uint32_t readlen;

  PSOCK_BEGIN(&s->sock);
  PSOCK_SEND_STR(&s->sock, http_header_200);
  PSOCK_SEND_STR(&s->sock, "Content-Type: ");
  PSOCK_SEND_STR(&s->sock, s->file->content_type);
  PSOCK_SEND_STR(&s->sock, "\r\n");
  PSOCK_SEND_STR(&s->sock, "Content-Length: ");
  itoa(s->file->size, (char*)s->buf, 10);
  PSOCK_SEND_STR(&s->sock, (const char*)s->buf);
  PSOCK_SEND_STR(&s->sock, "\r\n");
  PSOCK_SEND_STR(&s->sock, "\r\n");

  while (s->file_pos < s->file->size) {
    readlen = MIN(HTTPD_OUTBUF_SIZE, s->file->size - s->file_pos);
    flashsst25_readn(s->file->offset + s->file_pos, s->buf, readlen);
    s->file_pos += readlen;
    s->outbuf_pos = readlen;
    PSOCK_SEND(&s->sock, s->buf, s->outbuf_pos);
  }

  PSOCK_END(&s->sock);
}

PT_THREAD(serve_amps_set(process_event_t ev, struct httpd_state* s)) {
  uint32_t value;
  PSOCK_BEGIN(&s->sock);

  if (strncmp((const char*)s->buf, "value=", 6) == 0) {
    value = atoi((const char*)s->buf + 6);
    set_current_milliamps(value);

    PSOCK_SEND_STR(&s->sock, http_200_ok);
  } else {
    PSOCK_SEND_STR(&s->sock, http_400_fail);
  }

  PSOCK_END(&s->sock);
}

PT_THREAD(serve_web_socket(process_event_t ev, struct httpd_state* s)) {
  uint8_t i, len;
  uint8_t wsMask[4];
  char* p;

  PSOCK_BEGIN(&s->sock);
  PSOCK_SEND_STR(&s->sock, http_header_101_ws_upgrade);
  PSOCK_SEND_STR(&s->sock, "Sec-WebSocket-Accept: ");
  PSOCK_SEND_STR(&s->sock, s->sec_websocket_accept);
  PSOCK_SEND_STR(&s->sock, "\r\n\r\n");

  etimer_set(&s->ws_etimer, CLOCK_SECOND);

  while (!(uip_aborted() || uip_closed() || uip_timedout())) {
    PT_YIELD_UNTIL(&s->sock.pt, uip_aborted() || uip_closed() || uip_timedout() || uip_newdata() || etimer_expired(&s->ws_etimer));
    if (etimer_expired(&s->ws_etimer)) {
      p = (char*)&s->buf[2];
      strcpy(p, "{\"time\":");
      p += strlen(p);
      itoa(time_ms(), p, 10);
      strcat(p, ",\"voltage\":");
      p += strlen(p);
      itoa(get_millivolts(), p, 10);
      strcat(p, ",\"amperage\":");
      p += strlen(p);
      itoa(get_milliamps(), p, 10);
      strcat(p, ",\"targetAmps\":");
      p += strlen(p);
      itoa(get_set_milliamps(), p, 10);
      strcat(p, "}");

      len = strlen((const char*)&s->buf[2]);
      s->buf[0] = WS_FIN | WS_OPCODE_TEXT;
      s->buf[1] = len;
      s->outbuf_pos = len + 2;
      PSOCK_SEND(&s->sock, s->buf, s->outbuf_pos);

      etimer_reset(&s->ws_etimer);
    }
    if (uip_newdata()) {
      p = (char*)uip_appdata;
      if (p[0] == (WS_FIN | WS_OPCODE_TEXT)) {
        len = p[1] & 0x7f;
        if ((p[1] & WS_MASK) == WS_MASK) {
          memcpy(wsMask, &p[2], 4);
          for (i = 0; i < len; i++) {
            p[i] = p[i + 6] ^ wsMask[i % 4];
          }
        } else {
          for (i = 0; i < len; i++) {
            p[i] = p[i + 2];
          }
        }
        p[len] = '\0';
        debug_write("ws message: ");
        debug_write((char*)p);
        debug_write_line("");
        if (strcmp(p, "CLOSE") == 0) {
          break;
        }
      }
    }
  }

  PSOCK_END(&s->sock);
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

