
#ifndef _FLASH_FILES_H_
#define _FLASH_FILES_H_

#include <stdint.h>
#include "httpd.h"

extern char serve_flash_file(process_event_t ev, struct httpd_state* s);
extern char serve_amps_set(process_event_t ev, struct httpd_state* s);
extern char serve_web_socket(process_event_t ev, struct httpd_state* s);
extern char serve_recorder_start(process_event_t ev, struct httpd_state* s);
extern char serve_recorder_stop(process_event_t ev, struct httpd_state* s);
extern char serve_recorder_download(process_event_t ev, struct httpd_state* s);
extern char serve_info(process_event_t ev, struct httpd_state* s);

struct flashFile {
  const char* filename;
  const char* content_type;
  uint32_t offset;
  uint32_t size;
  httpd_script_t script;
};

extern struct flashFile flashFiles[];

#endif
