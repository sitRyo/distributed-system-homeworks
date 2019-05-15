#include <iostream>
#include <string>

#include <stdio.h>
using namespace std;

extern "C" {
  #include "global.h"
  #include "md5.h"

  void MDFile(char* filename, unsigned char digests[16]);
  int MD5Compare(unsigned char *digest, unsigned char *data, size_t data_len);
  void MDString(char *string, unsigned char digest[16]);
}

int main(int argc, char* argv[]) {
  unsigned char ary[16];
  char buf[1024];
  if (argc != 2) {
    fprintf(stderr, "input file name\n");
    exit(1);
  }
  FILE *f = fopen(argv[1], "rb");
  fread(buf, 1, 1024, f);
  MDString(buf, ary);
  for (int i = 0; i < 16; ++i) {
    printf("%02x ", ary[i]);
  }
  printf("\n");
}