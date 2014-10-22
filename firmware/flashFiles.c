
#include <stdlib.h>
#include "flashFiles.h"

static const char mimetype_application_javascript[] = "application/javascript";
static const char mimetype_image_png[] = "image/png";
static const char mimetype_text_css[] = "text/css";
static const char mimetype_text_html[] = "text/html";

  
struct flashFile flashFiles[] = {
  { .filename = "/js/jquery-2.1.1.min.js", .content_type = mimetype_application_javascript, .offset = 0, .size = 84245, .script = serve_flash_file },
  { .filename = "/js/bootstrap.min.js", .content_type = mimetype_application_javascript, .offset = 84245, .size = 31819, .script = serve_flash_file },
  { .filename = "/js/jquery.flot.time.min.js", .content_type = mimetype_application_javascript, .offset = 116064, .size = 6891, .script = serve_flash_file },
  { .filename = "/js/jquery.flot.min.js", .content_type = mimetype_application_javascript, .offset = 122955, .size = 52966, .script = serve_flash_file },
  { .filename = "/js/jquery.flot.axislabels.js", .content_type = mimetype_application_javascript, .offset = 175921, .size = 16940, .script = serve_flash_file },
  { .filename = "/js/dc-electronic-load.js", .content_type = mimetype_application_javascript, .offset = 192861, .size = 2309, .script = serve_flash_file },
  { .filename = "/js/ripples.min.js", .content_type = mimetype_application_javascript, .offset = 195170, .size = 1639, .script = serve_flash_file },
  { .filename = "/js/material.min.js", .content_type = mimetype_application_javascript, .offset = 196809, .size = 1900, .script = serve_flash_file },
  { .filename = "/css/material.min.css", .content_type = mimetype_text_css, .offset = 198709, .size = 96973, .script = serve_flash_file },
  { .filename = "/css/ripples.min.css", .content_type = mimetype_text_css, .offset = 295682, .size = 777, .script = serve_flash_file },
  { .filename = "/css/dc-electronic-load.css", .content_type = mimetype_text_css, .offset = 296459, .size = 44, .script = serve_flash_file },
  { .filename = "/css/bootstrap.min.css", .content_type = mimetype_text_css, .offset = 296503, .size = 109518, .script = serve_flash_file },
  { .filename = "/img/glyphicons-halflings.png", .content_type = mimetype_image_png, .offset = 406021, .size = 12799, .script = serve_flash_file },
  { .filename = "/img/glyphicons-halflings-white.png", .content_type = mimetype_image_png, .offset = 418820, .size = 8777, .script = serve_flash_file },
  { .filename = "/amps/set", .content_type = mimetype_text_html, .offset = 427597, .size = 0, .script = serve_amps_set },
  { .filename = "/ws", .content_type = mimetype_text_html, .offset = 427597, .size = 0, .script = serve_web_socket },
  { .filename = "/index.html", .content_type = mimetype_text_html, .offset = 427597, .size = 2002, .script = serve_flash_file },
  { .filename = NULL, .offset = 0, .size = 0 }
};
