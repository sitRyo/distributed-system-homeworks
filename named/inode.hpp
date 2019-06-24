#ifndef _INODE_
#define _INODE_ 1

#include <cstdint>

using int_t = uint16_t;  // intは本当にsigned 32bitか？

struct INode{
  int_t i_mode;
  char i_nlink;
  char i_uid;
  char i_gid;
  char i_size0;
  int_t i_size1;
  int_t i_addr[8];
  int_t i_atime[2];
  int_t i_mtime[2];
};

/* modes */
#define IALLOC 0100000
#define IFMT 060000
#define IFDIR 040000
#define IFCHR 020000
#define IFBLK 060000
#define ILARG 010000
#define ISUID 04000
#define ISGID 02000
#define ISVTX 01000
#define IREAD 0400
#define IWRITE 0200
#define IEXEC 0100

#endif // _INODE_