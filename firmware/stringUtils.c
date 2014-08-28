#include "stringUtils.h"
#include <string.h>

void addCommas(const char* in, char* out) {
  int inlen = strlen(in);
  const char* pin = in + inlen;
  char* pout = out + inlen + ((inlen - 1) / 3);
  int count = 0;
  *pout-- = *pin--;
  while (pin >= in) {
    if (count == 3) {
      *pout-- = ',';
      count = 0;
    }
    *pout-- = *pin--;
    count++;
  }
}

void padLeft(const char* in, char* out, int len, char ch) {
  int i;
  int inlen = strlen(in);
  const char* pin = in;
  char* pout = out;
  int spacesNeeded = len - inlen;
  for (i = 0; i < spacesNeeded; i++) {
    *pout++ = ch;
  }
  while (*pin) {
    *pout++ = *pin++;
  }
  *pout++ = '\0';
}

char* itoa(int32_t value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) {
    *result = '\0';
    return result;
  }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while (value);

  // Apply negative sign
  if (tmp_value < 0) {
    *ptr++ = '-';
  }
  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

char* uitoa(uint32_t value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) {
    *result = '\0';
    return result;
  }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while (value);

  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}
