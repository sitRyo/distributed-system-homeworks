#ifndef _INC_LOGGER_
#define _INC_LOGGER_ 1

#include "Util.hpp"

#include <iostream>
#include <unordered_map>
#include <string>

class Logger {
/* :members */
  std::unordered_map<short, std::string> _logMessageTable;
  void registerMessage() noexcept;
public:
/* :constructors */
  Logger() noexcept;
/* :destructors */
  ~Logger() = default;
  void showMessage(const short flag, const double time) const noexcept;
};

#endif // _INC_LOGGER_  1
