#ifndef _ANALIZER_
#define _ANALIZER_ 1

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "super_block.hpp" // for super node
#include "inode.hpp" // for inode 
#include "mount.hpp" // for storage
#include "name_t.hpp" // for name

struct Analyzer {
  SuperBlock *sblock = nullptr;
  char *buffer = nullptr;

  void open_storage_file( std::string file_name );
  int get_storage_length( std::ifstream& ifs );

  void seek_bootblock();
  void seek_superblock();
  void get_superblock( char* buf, SuperBlock* block );
  void get_inodeblock( int ino, INode* inode );
  void get_storageblock( int_t ino, char *buf );
  void deserialize_inode( char* tp, INode* inode );
  void dump_superblock( SuperBlock *block );
  bool isDirectory( int_t t );
  std::vector<name_t> get_child_directories( char* buf );
  std::vector<name_t> seek_storage( INode& inode );

  std::string check_permission( int inode_number );

  uint16_t byte_swap_2( uint16_t t );
};

#endif // _ANALIZER_