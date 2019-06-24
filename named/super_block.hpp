#ifndef _SUPERBLOCK_
#define _SUPERBLOCK_ 1

#include <cstdint>

using int_t = uint16_t; // intは本当にsigned 32bitか？

struct SuperBlock{
  int_t s_isize;
  int_t s_fsize;
  int_t s_nfree;
  int_t s_free[100];
  int_t s_ninode;
  int_t s_inode[100];
  char s_flock;
  char s_ilock;
  char s_fmod;
  char s_ronly;
  int_t s_time[2];
  int_t pad[50];
};

#endif // _SUPERBLOCK_