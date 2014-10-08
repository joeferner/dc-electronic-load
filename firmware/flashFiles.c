
#include <stdlib.h>
#include "flashFiles.h"

static const char mimetype_application_javascript[] = "application/javascript";
static const char mimetype_text_html[] = "text/html";

  
struct flashFile flashFiles[] = {
  { .filename = "/js/jquery-2.1.1.min.js", .content_type = mimetype_application_javascript, .offset = 0, .size = 84245, .script = serve_flash_file },
  { .filename = "/js/dc-electronic-load.js", .content_type = mimetype_application_javascript, .offset = 84245, .size = 594, .script = serve_flash_file },
  { .filename = "/index.html", .content_type = mimetype_text_html, .offset = 0, .size = 278, .script = serve_flash_file },
  { .filename = NULL, .offset = 0, .size = 0 }
};
