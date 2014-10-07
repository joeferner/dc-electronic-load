
#include <stdlib.h>
#include "flashFiles.h"

struct flashFile flashFiles[] = {
  { .filename = "/jquery-2.1.1.min.js", .offset = 0, .size = 84245 },
  { .filename = "/dc-electronic-load.js", .offset = 84245, .size = 44 },
  { .filename = "/index.html", .offset = 84289, .size = 260 },
  { .filename = NULL, .offset = 0, .size = 0 }
};
