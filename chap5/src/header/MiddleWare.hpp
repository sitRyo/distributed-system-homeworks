#ifndef _INC_MIDDLEWARE_
#define _INC_MIDDLEWARE_ 1

#include "Sender.hpp"
#include "Receiver.hpp"
#include "Util.h"
#include "Task.hpp"
#include "Logger.hpp"

#include <memory>
#include <string>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <thread>

class Sender;
class Receiver;

class MiddleWare {
private:
/* :members */
  Logger _logger;
  std::mutex _mtx;
  std::unique_ptr<Receiver> _receiver;
  std::unique_ptr<Sender> _sender;
  std::chrono::system_clock::time_point  _start;
  double _elapsed;
  std::unordered_map<std::string, int> _nameToSocketIndex;
  int _senderHasSockets;
public:
/* :construtors */
  MiddleWare() noexcept;
  ~MiddleWare() = default;
/* methods */
  bool AddNewClient(const std::string&& opponent, const std::string&& ipaddr, const int port) noexcept;
  void updateClock() noexcept;
  void showSocketTable() const;
  bool sendData(const std::string&& opponent, Task& task); // 今回は妥協
  bool terminate(const std::string&& opponent);
  void notify(char *buf); // observer
};

#endif // _INC_MIDDLEWARE_ 