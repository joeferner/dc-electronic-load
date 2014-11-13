
#include "network.h"
#include "debug.h"
#include "httpd.h"
#include "flashFiles.h"
#include "sha1.h"
#include "base64.h"
#include "net/ip/tcpip.h"
#include <stdlib.h>

static const char http_10[] = " HTTP/1.0\r\n";
static const char http_content_type[] = "Content-Type:";
static const char http_content_type_html[] = "text/html";
static const char http_content_len[] = "Content-Length:";
static const char http_sec_websocket_key[] = "Sec-WebSocket-Key:";
static const char http_websocket_guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
static const char http_header_404[] = "HTTP/1.0 404 Not found\r\nConnection: close\r\n";
static const char html_not_found[] = "<html><body><h1>Page not found</h1></body></html>";

uint16_t http_connections = 0;
static struct httpd_state conns[HTTPD_CONNS];

void httpd_init();
void httpd_appcall(process_event_t ev, void* state);
void httpd_state_init();
struct httpd_state* httpd_state_alloc();
void httpd_state_free(struct httpd_state* s);
void httpd_handle_connection(process_event_t ev, struct httpd_state* s);

PROCESS(httpd_process, "HTTP server");

PROCESS_THREAD(httpd_process, ev, data) {
  static struct etimer et;
  int i;

  PROCESS_BEGIN();

  httpd_init();

  /* Delay 2-4 seconds */
  etimer_set(&et, CLOCK_SECOND * 10);

  /* GC any http session that is too long lived - either because other
     end never closed or if any other state cause too long lived http
     sessions */
  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event || etimer_expired(&et));
    if (ev == tcpip_event) {
      httpd_appcall(ev, data);
    } else if (etimer_expired(&et)) {
      debug_write("HTTPD States: ");
      for (i = 0; i < HTTPD_CONNS; i++) {
        debug_write_u8(conns[i].state, 16);
        if (conns[i].state != HTTPD_STATE_UNUSED && timer_expired(&conns[i].timer)) {
          conns[i].state = HTTPD_STATE_UNUSED;
          debug_write("*** RELEASED HTTPD Session");
          http_connections--;
        }
      }
      debug_write_line("");
      etimer_reset(&et);
    }
  }

  PROCESS_END();
}

void httpd_init() {
  debug_write_line("?httpd_init");
  tcp_listen(UIP_HTONS(80));
  httpd_state_init();
}

void httpd_appcall(process_event_t ev, void* state) {
  struct httpd_state* s = (struct httpd_state*)state;

  if (uip_closed() || uip_aborted() || uip_timedout()) {
    if (s != NULL) {
      debug_write("HTTPD: closed/aborted: ");
      if (s->file) {
        debug_write(s->file->filename);
      }
      debug_write_line("");
      http_connections--;
      httpd_state_free(s);
    } else {
      debug_write_line("HTTPD: closed/aborted ** NO HTTPD_WS_STATE!!! **");
    }
  } else if (uip_connected()) {
    if (s == NULL) {
      s = httpd_state_alloc();
      if (s == NULL) {
        uip_abort();
        debug_write_line("HTTPD: aborting - no resource");
        return;
      }
      http_connections++;

      tcp_markconn(uip_conn, s);
      s->state = HTTPD_STATE_INPUT;
    } else {
      /* this is a request that is to be sent! */
      s->state = HTTPD_STATE_REQUEST_OUTPUT;
    }
    PSOCK_INIT(&s->sock, (uint8_t*)s->buf, sizeof(s->buf) - 1);
    PT_INIT(&s->outputpt);
    timer_set(&s->timer, CLOCK_SECOND * 30);
    httpd_handle_connection(ev, s);
  } else if (s != NULL) {
    if (uip_poll()) {
      if (timer_expired(&s->timer)) {
        uip_abort();
        debug_write_line("HTTPD: aborting - http timeout");
        http_connections--;
        httpd_state_free(s);
      }
    } else {
      timer_restart(&s->timer);
    }
    httpd_handle_connection(ev, s);
  } else {
    debug_write_line("HTTPD: aborting - no state");
    uip_abort();
  }
}

PT_THREAD(send_string(struct httpd_state* s, const char* str, uint16_t len)) {
  PSOCK_BEGIN(&s->sock);
  SEND_STRING(&s->sock, str, len);
  PSOCK_END(&s->sock);
}

PT_THREAD(send_headers(struct httpd_state* s, const char* statushdr)) {
  PSOCK_BEGIN(&s->sock);

  SEND_STRING(&s->sock, statushdr, strlen(statushdr));
  strcpy((char*)s->buf, http_content_type);
  strcat((char*)s->buf, " ");
  strcat((char*)s->buf, (s->file == NULL || s->file->content_type == NULL) ? http_content_type_html : s->file->content_type);
  strcat((char*)s->buf, "\r\n\r\n");
  s->outbuf_pos = strlen((char*)s->buf);
  SEND_STRING(&s->sock, s->buf, s->outbuf_pos);
  s->outbuf_pos = 0;

  PSOCK_END(&s->sock);
}

void httpd_calculate_websocket_accept(const char* key, char* accept) {
  SHA1Context ctx;
  int i, j;
  uint8_t data[20];

  SHA1Reset(&ctx);
  SHA1Input(&ctx, (uint8_t*)key, strlen(key));
  SHA1Input(&ctx, (uint8_t*)http_websocket_guid, strlen(http_websocket_guid));
  SHA1Result(&ctx);

  for (i = 0, j = 0; i < 5; i++, j += 4) {
    data[j + 0] = (ctx.Message_Digest[i] >> 24) & 0xff;
    data[j + 1] = (ctx.Message_Digest[i] >> 16) & 0xff;
    data[j + 2] = (ctx.Message_Digest[i] >> 8) & 0xff;
    data[j + 3] = (ctx.Message_Digest[i] >> 0) & 0xff;
  }

  base64_encode(data, 20, accept);
}

PT_THREAD(httpd_handle_input(process_event_t ev, struct httpd_state* s)) {
  PSOCK_BEGIN(&s->sock);
  PSOCK_READTO(&s->sock, ' ');

  if (strncmp((char*)s->buf, "GET ", 4) == 0) {
    s->request_type = HTTPD_GET;
  } else if (strncmp((char*)s->buf, "POST ", 5) == 0) {
    s->request_type = HTTPD_POST;
    s->content_len = 0;
  } else if (strncmp((char*)s->buf, "HTTP ", 5) == 0) {
    s->request_type = HTTPD_RESPONSE;
  } else {
    PSOCK_CLOSE_EXIT(&s->sock);
  }
  PSOCK_READTO(&s->sock, ' ');

  if (s->buf[0] != '/') {
    PSOCK_CLOSE_EXIT(&s->sock);
  }

  s->buf[PSOCK_DATALEN(&s->sock) - 1] = 0;
  if (strcmp((const char*)s->buf, "/") == 0) {
    strcpy((char*)s->buf, "/index.html");
  }
  s->file = httpd_get_file((const char*)s->buf);
  s->file_pos = 0;
  debug_write("?httpd file: ");
  debug_write((const char*)s->buf);
  debug_write_line("");

  while (1) {
    PSOCK_READTO(&s->sock, '\n');

    if (s->request_type == HTTPD_POST && strncmp((char*)s->buf, http_content_len, 15) == 0) {
      s->buf[PSOCK_DATALEN(&s->sock) - 2] = 0;
      s->content_len = atoi((char*)&s->buf[16]);
    }

    if (strncmp((char*)s->buf, http_sec_websocket_key, 18) == 0) {
      s->buf[PSOCK_DATALEN(&s->sock) - 2] = 0;
      httpd_calculate_websocket_accept((char*)&s->buf[19], s->sec_websocket_accept);
    }

    /* should have a header callback here check_header(s) */
    if (PSOCK_DATALEN(&s->sock) > 2) {
      s->buf[PSOCK_DATALEN(&s->sock) - 2] = 0;
    } else if (s->request_type == HTTPD_POST) {
      if (s->content_len > 0) {
        PSOCK_READBUF_LEN(&s->sock, s->content_len);
        s->buf[PSOCK_DATALEN(&s->sock)] = 0;
      } else {
        s->buf[0] = 0;
      }
      s->state = HTTPD_STATE_OUTPUT;
      break;
    } else {
      s->state = HTTPD_STATE_OUTPUT;
      break;
    }
  }
  PSOCK_END(&s->sock);
}

PT_THREAD(httpd_handle_output(process_event_t ev, struct httpd_state* s)) {
  PT_BEGIN(&s->outputpt);

  if (s->file == NULL) {
    PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_404));
    PT_WAIT_THREAD(&s->outputpt, send_string(s, html_not_found, strlen(html_not_found)));
  } else {
    PT_WAIT_THREAD(&s->outputpt, s->file->script(ev, s));
  }
  PSOCK_CLOSE(&s->sock);
  PT_END(&s->outputpt);
}

void httpd_handle_connection(process_event_t ev, struct httpd_state* s) {
  if (s->state == HTTPD_STATE_INPUT) {
    httpd_handle_input(ev, s);
  }
  if (s->state == HTTPD_STATE_OUTPUT) {
    httpd_handle_output(ev, s);
  }
}

void httpd_state_init() {
  int i;

  for (i = 0; i < HTTPD_CONNS; i++) {
    conns[i].state = HTTPD_STATE_UNUSED;
  }
}

struct httpd_state* httpd_state_alloc() {
  int i;

  for (i = 0; i < HTTPD_CONNS; i++) {
    if (conns[i].state == HTTPD_STATE_UNUSED) {
      conns[i].state = HTTPD_STATE_INPUT;
      return &conns[i];
    }
  }
  return NULL;
}

void httpd_state_free(struct httpd_state* s) {
  s->state = HTTPD_STATE_UNUSED;
}

