
#include <stdlib.h>
#include "flashFiles.h"

struct flashFile flashFiles[] = {
  {
    .filename = "index.html",
    .offset = 0,
    .size = 1000
  },
  {
    .filename = NULL,
    .offset = 0,
    .size = 0
  }
};
