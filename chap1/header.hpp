#ifndef _HEADER_
#define _HEADER_

#include <cstdint>
#include <cstddef>

extern "C" {
  #include "global.h"
  #include "md5.h"

  void MDFile(const char* filename, unsigned char digests[16]);
  void MDString(char *string, unsigned char digest[16]);
}

constexpr std::size_t packet_size_max = 1024;

enum LINK_LAYER_TYPE {
  TCP = 0,
  UDP
};

typedef struct dip {
  int type;
  int version;
  int ttl;
} DIP;

constexpr std::size_t dip_size = sizeof(dip);

typedef struct dtcp {
  int type;
  int len;
  unsigned char t[16];
} DTCP;

constexpr std::size_t dtcp_size = sizeof(dtcp);

typedef struct dudp {
  int type;
  int len;
} DUDP;

constexpr std::size_t dudp_size = sizeof(dudp);


#endif