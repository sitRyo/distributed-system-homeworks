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
  int _uniqueNumber;
  Logger _logger;
  std::mutex _mtx;
  // std::unique_ptr<Receiver> _receiver;
  std::unique_ptr<Sender> _sender;
  std::chrono::system_clock::time_point  _start;
  double _elapsed;
  std::unordered_map<std::string, int> _nameToSocketIndex;
  std::unordered_map<int, std::string> _uniqueNumberToString;
  int _senderHasSockets;
  std::unique_ptr<std::thread> _receiver;
  int _countAckNum = 0;
public:
  std::vector<int> _tasks;

/* :construtors */
  MiddleWare(const int unique_number) noexcept;
  ~MiddleWare();
/* methods */
  bool AddNewClient(const int num, const std::string opponent, const std::string&& ipaddr, const int port) noexcept;
  void updateClock() noexcept;
  void showSocketTable() const;
  bool sendData(const std::string&& opponent, Task& task); // 今回は妥協
  bool terminate(const std::string&& opponent);
  void notify(int threadNumber, char *buf); // observer
  int getUniqueNumber() const noexcept;
  std::string uniqueToString(const int unique);
  bool sendAck(const std::string opponent);
  bool countAckNum() noexcept;
  void executeTasks() noexcept;
};

#endif // _INC_MIDDLEWARE_ 