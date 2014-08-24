
#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <stdint.h>

void addCommas(const char* in, char* out);
void padLeft(const char* in, char* out, int len);
char* itoa(int32_t value, char* result, int base);
char* uitoa(uint32_t value, char* result, int base);

#endif // _STRING_UTILS_H_
