
#include "network.h"
#include "debug.h"
#include "httpd.h"
#include <stdlib.h>

static const char http_10[] = " HTTP/1.0\r\n";
static const char http_content_type[] = "Content-Type:";
static const char http_content_type_html[] = "text/html";
static const char http_content_len[] = "Content-Length:";
static const char http_header_404[] = "HTTP/1.0 404 Not found\r\nServer: Contiki\r\nConnection: close\r\n";
static const char http_header_200[] = "HTTP/1.0 200 OK\r\nServer: Contiki\r\nConnection: close\r\n";
static const char html_not_found[] = "<html><body><h1>Page not found</h1></body></html>";

uint16_t http_connections = 0;
static struct httpd_state conns[HTTPD_CONNS];

void httpd_init();
void httpd_appcall(void* state);
void httpd_state_init();
struct httpd_state* httpd_state_alloc();
void httpd_state_free(struct httpd_state* s);
void httpd_handle_connection(struct httpd_state* s);

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
      httpd_appcall(data);
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

void httpd_appcall(void* state) {
  struct httpd_state* s = (struct httpd_state*)state;

  if (uip_closed() || uip_aborted() || uip_timedout()) {
    if (s != NULL) {
      debug_write_line("HTTPD: closed/aborted");
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
    PSOCK_INIT(&s->sin, (uint8_t*)s->inputbuf, sizeof(s->inputbuf) - 1);
    PSOCK_INIT(&s->sout, (uint8_t*)s->inputbuf, sizeof(s->inputbuf) - 1);
    PT_INIT(&s->outputpt);
    timer_set(&s->timer, CLOCK_SECOND * 30);
    httpd_handle_connection(s);
  } else if (s != NULL) {
    if (uip_poll()) {
      if (timer_expired(&s->timer)) {
        uip_abort();
        debug_write_line("HTTPD: aborting - http timeout");
        http_connections--;
        httpd_state_free(s);
      } else {
        debug_write_line("HTTPD: uip-poll");
      }
    } else {
      timer_restart(&s->timer);
    }
    httpd_handle_connection(s);
  } else {
    debug_write_line("HTTPD: aborting - no state");
    uip_abort();
  }
}

PT_THREAD(send_string(struct httpd_state* s, const char* str, uint16_t len)) {
  PSOCK_BEGIN(&s->sout);
  SEND_STRING(&s->sout, str, len);
  PSOCK_END(&s->sout);
}

PT_THREAD(send_headers(struct httpd_state* s, const char* statushdr)) {
  PSOCK_BEGIN(&s->sout);

  SEND_STRING(&s->sout, statushdr, strlen(statushdr));
  strcpy((char*)s->outbuf, http_content_type);
  strcat((char*)s->outbuf, " ");
  strcat((char*)s->outbuf, s->content_type == NULL ? http_content_type_html : s->content_type);
  strcat((char*)s->outbuf, "\r\n\r\n");
  s->outbuf_pos = strlen((char*)s->outbuf);
  SEND_STRING(&s->sout, s->outbuf, s->outbuf_pos);
  s->outbuf_pos = 0;

  PSOCK_END(&s->sout);
}

PT_THREAD(httpd_handle_input(struct httpd_state* s)) {
  PSOCK_BEGIN(&s->sin);
  PSOCK_READTO(&s->sin, ' ');

  if (strncmp((char*)s->inputbuf, "GET ", 4) == 0) {
    s->request_type = HTTPD_GET;
  } else if (strncmp((char*)s->inputbuf, "POST ", 5) == 0) {
    s->request_type = HTTPD_POST;
    s->content_len = 0;
  } else if (strncmp((char*)s->inputbuf, "HTTP ", 5) == 0) {
    s->request_type = HTTPD_RESPONSE;
  } else {
    PSOCK_CLOSE_EXIT(&s->sin);
  }
  PSOCK_READTO(&s->sin, ' ');

  if (s->inputbuf[0] != '/') {
    PSOCK_CLOSE_EXIT(&s->sin);
  }

  s->inputbuf[PSOCK_DATALEN(&s->sin) - 1] = 0;
  if (strcmp((const char*)s->inputbuf, "/") == 0) {
    strcpy((char*)s->inputbuf, "/index.html");
  }
  s->file = httpd_get_file((const char*)s->inputbuf);
  s->file_pos = 0;
  debug_write("?httpd: ");
  debug_write((const char*)s->inputbuf);
  debug_write(" ");
  debug_write_u32((uint32_t)s->file, 10);
  debug_write_line("");

  s->state = HTTPD_STATE_OUTPUT;

  while (1) {
    PSOCK_READTO(&s->sin, '\n');

    if (s->request_type == HTTPD_POST && strncmp((char*)s->inputbuf, http_content_len, 15) == 0) {
      s->inputbuf[PSOCK_DATALEN(&s->sin) - 2] = 0;
      s->content_len = atoi((char*)&s->inputbuf[16]);
    }

    /* should have a header callback here check_header(s) */
    if (PSOCK_DATALEN(&s->sin) > 2) {
      s->inputbuf[PSOCK_DATALEN(&s->sin) - 2] = 0;
    } else if (s->request_type == HTTPD_POST) {
      PSOCK_READBUF_LEN(&s->sin, s->content_len);
      s->inputbuf[PSOCK_DATALEN(&s->sin)] = 0;
      s->state = HTTPD_STATE_OUTPUT;
    }
  }
  PSOCK_END(&s->sin);
}

PT_THREAD(httpd_handle_output(struct httpd_state* s)) {
  PT_BEGIN(&s->outputpt);

  s->content_type = http_content_type_html;
  if (s->file != NULL) {
    s->script = httpd_get_script(s);
  }
  if (s->script == NULL || s->file == NULL) {
    PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_404));
    PT_WAIT_THREAD(&s->outputpt, send_string(s, html_not_found, strlen(html_not_found)));
    uip_close();
    PT_EXIT(&s->outputpt);
  } else {
    if (s->request_type == HTTPD_POST) {
      /* A post has a body that needs to be read */
      s->state = HTTPD_STATE_INPUT;
      PT_WAIT_UNTIL(&s->outputpt, s->state == HTTPD_STATE_OUTPUT);
    }
    PT_WAIT_THREAD(&s->outputpt, send_headers(s, http_header_200));
    PT_WAIT_THREAD(&s->outputpt, s->script(s));
  }
  s->script = NULL;
  PSOCK_CLOSE(&s->sout);
  PT_END(&s->outputpt);
}

void httpd_handle_connection(struct httpd_state* s) {
  httpd_handle_input(s);
  if (s->state == HTTPD_STATE_OUTPUT) {
    httpd_handle_output(s);
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

