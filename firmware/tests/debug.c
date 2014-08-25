#include <stdio.h>

void debug_write_line(const char* str) {
  printf("%s\n", str);
}

void debug_write(const char* str) {
  printf("%s", str);
}
