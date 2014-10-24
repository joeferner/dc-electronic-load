
#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdint.h>

void base64_encode(const unsigned char* data, uint16_t input_length, char* encoded_data);

#endif
