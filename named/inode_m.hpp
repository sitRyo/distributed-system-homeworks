#ifndef _INODE_M_
#define _INODE_M_ 1

#include <cstdint>

using int_t = int64_t;

struct inode_m
{
  char i_flag;
  char i_count;
  int_t i_dev;
  int_t i_number;
  int_t i_mode;
  char i_nlink;
  char i_uid;
  char i_gid;
  char i_size0;
  char *i_size1;
  int_t i_addr[8];
  int_t i_lastr;
};

/* flags */
#define ILOCK 01
#define IUPD 02
#define IACC 04
#define IMOUNT 010
#define IWANT 020
#define ITEXT 040

/* modes */
#define IALLOC 0100000
#define IFMT 060000


#endif //_INODE_M_