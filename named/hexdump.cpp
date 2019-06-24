#include <iostream>
#include "hexdump.hpp"

void hex_dump( char* str, int length ) {
  for (int i = 0; i < length; ++i) {
    if (i % 16 == 0) puts("");
    printf("%02x ", str[i] & 0xff);
  }
  puts("");
}

void hex_dump_char( char* str, int length ) {
  for (int i = 0; i < length; ++i) {
    if (i % 16 == 0) puts("");
    printf("%c ", str[i]);
  }
  puts("");
}