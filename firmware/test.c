#include "stringUtils.h"
#include <string.h>
#include <stdio.h>

void testAddCommas(const char* src, const char* expected);
void testPadLeft(const char* src, const char* expected, int totalLen);

int main(int argc, const char* argv[]) {
  testAddCommas("0", "0");
  testAddCommas("1", "1");
  testAddCommas("10", "10");
  testAddCommas("123", "123");
  testAddCommas("1234", "1,234");
  testAddCommas("12345", "12,345");
  testAddCommas("123456", "123,456");
  testAddCommas("1234567", "1,234,567");
  testPadLeft("123", "123", 2);
  testPadLeft("123", "123", 3);
  testPadLeft("123", " 123", 4);

  return 0;
}

void testAddCommas(const char* src, const char* expected) {
  char temp[20];

  addCommas(src, temp);
  if(strcmp(temp, expected) == 0) {
    printf("OK: %s -> %s\n", src, temp);
  } else {
    printf("FAIL: %s -> %s\n", src, temp);
  }
}

void testPadLeft(const char* src, const char* expected, int totalLen) {
  char temp[20];

  padLeft(src, temp, totalLen);
  if(strcmp(temp, expected) == 0) {
    printf("OK: %s -> %s\n", src, temp);
  } else {
    printf("FAIL: %s -> %s\n", src, temp);
  }
}
