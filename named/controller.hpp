#ifndef _INC_CONTROLLER_
#define _INC_CONTROLLER_ 1

#include <cstdint>
#include <string>
#include <vector>
#include "analyzer.hpp"

using int_t = uint16_t;

class V6Sh {
  Analyzer analyzer;
  std::string file_name;
public:
  V6Sh( std::string& ); // 右辺値でよくない？
  void init();
  void start();
  std::vector<name_t> get_child_directries( INode& inode );
  std::string get_command( std::string& input );
  std::string get_next_dir( std::string& input );
};

#endif // _INC_CONTROLLER_