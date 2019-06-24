#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <cassert>

#include "analyzer.hpp" // prototype
#include "hexdump.hpp" // for hexdump

#pragma pack(1)

// ファイルを開く。
void Analyzer::open_storage_file( std::string file_name ) {
  std::ifstream ifs(file_name, std::ios::binary);
  if (ifs.fail()) {
    fprintf(stderr, "cannot open the file.");
    exit(1);
  }
  int length = get_storage_length(ifs);
  // printf("len %d\n", length);
  buffer = new char[length];
  ifs.read(buffer, length);
}

// ファイルの大きさを返す。
int Analyzer::get_storage_length( std::ifstream& ifs ) {
  ifs.seekg(0, ifs.end);
  int length = ifs.tellg();
  ifs.seekg(0, ifs.beg);
  return length;
}

// bootblockをseekする。 BLOCK_SIZE = 512, seek後のポインタは512を指す。
void Analyzer::seek_bootblock() {
  char buf[BLOCK_SIZE];
  memcpy(buf, buffer, BLOCK_SIZE);
}

// superblockをseekする。 seek後のポインタはBLOCKSIZE * 2を指す。
void Analyzer::seek_superblock() {
  char buf[BLOCK_SIZE];
  sblock = new SuperBlock[BLOCK_SIZE];
  memset(reinterpret_cast<void*>(sblock), 0, sizeof(SuperBlock));
  memcpy(buf, &buffer[BLOCK_SIZE], BLOCK_SIZE);
  // hex_dump(&buffer[BLOCK_SIZE], BLOCK_SIZE);
  get_superblock(buf, sblock);
  // dump_superblock(sblock);
}

// i番目のinodeblockを読み出す。
void Analyzer::get_inodeblock( int ino, INode* inode ) {
  const int inode_start_block = 2;
  const int ino_start_block = ((ino + 31) / 16);
  const int ino_start_address = (ino_start_block) * BLOCK_SIZE + (32 * ((ino + 31) % 16));
  // printf("ino %d start %d\n", ino, ino_start_address);
  char tp[INODE_SIZE];
  memcpy(tp, &buffer[ino_start_address], INODE_SIZE);
  // hex_dump(tp, INODE_SIZE);
  deserialize_inode(tp, inode);
}

// inodeブロックをstruct inodeにデシリアライズ
void Analyzer::deserialize_inode( char* tp, INode* inode ) {
  int p = 0;
  inode->i_mode = byte_swap_2(*(int_t*)(tp)); p += 2;
  inode->i_nlink = tp[p++];
  inode->i_uid = tp[p++];
  inode->i_gid = tp[p++];
  inode->i_size0 = tp[p++];
  inode->i_size1 = byte_swap_2(*(int_t*)(&tp[p])); p += 2;
  for (int i = 0; i < 8; ++i) {inode->i_addr[i] = byte_swap_2(*(int_t*)(&tp[p])); p += 2; }
  for (int i = 0; i < 2; ++i) {inode->i_atime[i] = byte_swap_2(*(int_t*)(&tp[p])); p += 2; }
  for (int i = 0; i < 2; ++i) {inode->i_mtime[i] = byte_swap_2(*(int_t*)(&tp[p])); p += 2; }
}

// inodeのアドレスからストレージを見る
void Analyzer::get_storageblock( int_t ino, char *buf ) {
  if (ino == 0) return; 
  // printf("%d\n", ino);
  memcpy(buf, &buffer[ino * 2], BLOCK_SIZE);
  // hex_dump_char(&buffer[ino * 2], BLOCK_SIZE);
  // hex_dump(&buffer[ino * 2], BLOCK_SIZE);
}

// super block をフェッチする.
void Analyzer::get_superblock( char* buf, SuperBlock* block ) {
  int p = 0;
  block->s_isize = byte_swap_2(*(int_t*)(buf)); p += 2;
  block->s_fsize = byte_swap_2(*(int_t*)(&buf[p])); p += 2;
  block->s_nfree = byte_swap_2(*(int_t*)(&buf[p])); p += 2;
  for (int i = 0; i < 100; ++i) { block->s_free[i] = byte_swap_2(*(int_t*)(&buf[p])); p += 2;}
  block->s_ninode = byte_swap_2(*(int_t*)(&buf[p])); p += 2;
  for (int i = 0; i < 100; ++i) { block->s_inode[i] = byte_swap_2(*(int_t*)(&buf[p])); p += 2; } 
  // skip char s_flock, s_ilock, s_fmod, s_ronly 
  // printf("%02x\n", buf[p]);
  block->s_flock = buf[p++];
  block->s_ilock = buf[p++];
  block->s_fmod = buf[p++];
  block->s_ronly = buf[p++];
  // printf("%d\n", p);
  for (int i = 0; i < 2; ++i) { block->s_time[i] = byte_swap_2(*(int_t*)(&buf[p])); p += 2; }
  // skip padding
}

void Analyzer::dump_superblock( SuperBlock* block ) {
  printf("%d\n", sizeof(SuperBlock));
  printf("%d\n", block->s_isize);
  printf("%d\n", block->s_fsize);
  printf("%d\n", block->s_nfree);
  for (int i = 0; i < 100; ++i) {
    printf("%d ", block->s_free[i]);
  }
  printf("\n%d\n", block->s_ninode);
  for (int i = 0; i < 100; ++i) {
    printf("%d ", block->s_inode[i]);
  }
  printf("\n%02x\n", block->s_flock);
  printf("%02x\n", block->s_ilock);
  printf("%02x\n", block->s_fmod );
  printf("%02x\n", block->s_ronly);
  for (int i = 0; i < 2; ++i) {
    printf("%d ", block->s_time[i]);
  }
  printf("\n");
}

std::vector<name_t> Analyzer::seek_storage( INode& inode ) {
  std::vector<name_t> names;
  for (int i = 0; i < 8; ++i) {
    char buf[BLOCK_SIZE];
    get_storageblock(inode.i_addr[i], buf);
    for (auto&& itr : get_child_directories(buf)) {
      names.emplace_back(itr);
    }
  }

  return names;
}

// directoryのinodeからファイルを得る。
std::vector<name_t> Analyzer::get_child_directories( char* buf ) {
  std::vector<name_t> names;
  for (int i = 0; i < 32; ++i) {
    name_t name;
    name.ino = *(int_t*)(&buf[i*16]);
    if (name.ino == 0) break; // inodeが0
    for (int j = 2; j < 16; ++j) {
      name.name.push_back(buf[j + i * 16]); // 文末は'\0'
      if (buf[j + i*16] == '\0') break;
    }
    names.emplace_back(name);
  }
  return names;
}

uint16_t Analyzer::byte_swap_2( uint16_t t ) {
  uint16_t p = ((t & 0x00ff) << 8) | ((t & 0xff00) >> 8);
  return p;
}

bool Analyzer::isDirectory( int_t t ) {
  // printf("%d\n", (t & IFDIR) == IFDIR);
  return (t & IFDIR) == IFDIR;
}

std::string Analyzer::check_permission( int inode_number ) {
  INode inode;
  std::string ret = "";
  std::string permission = "";

  get_inodeblock(inode_number, &inode);
  if (isDirectory(inode.i_mode)) ret += "d";
  for (int i = 2; i >= 0; --i) {
    ret += ((inode.i_mode >> i * 3) & IREAD) ? "r" : "-";
    ret += ((inode.i_mode >> i * 3) & IWRITE) ? "w" : "-";
    ret += ((inode.i_mode >> i * 3) & IEXEC) ? "x" : "-";
  }
  ret += "          ";
  ret += std::to_string(static_cast<int>(inode.i_size0) + static_cast<int>(inode.i_size1 + 511));
  return ret + " ";
}