#ifndef _INC_NAME_
#define _INC_NAME_ 1

#include <cstdint>
#include <string>

using int_t = uint16_t;

struct name_t {
  int_t ino;
  std::string name;
};

#endif // _INC_NAME_