#include "header/MiddleWare.hpp"
#include <iostream>
#include <type_traits>

MiddleWare::MiddleWare() noexcept {
  _start = std::chrono::system_clock::now();
  _elapsed = 0.0;
  _sender.reset(new Sender(this));
  _receiver.reset(new Receiver(this));
  _senderHasSockets = 0;
};

void MiddleWare::updateClock() noexcept {
  const std::chrono::system_clock::time_point end 
    = std::chrono::system_clock::now();
  _elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-_start).count();
}

bool MiddleWare::AddNewClient(const std::string&& opponent, const std::string&& ipaddr, const int port) noexcept {
  int index = _sender->AddNewClient(std::move(ipaddr), port);
  if (index != -1) {
    _nameToSocketIndex[opponent] = index;
    return true;
  } else {
    return false;
  }
}

void MiddleWare::showSocketTable() const {
  std::cout << "----show connecting sockets----" << std::endl;
  for (auto&& itr : _nameToSocketIndex) {
    std::cout << itr.first << std::endl;
  }
}

bool MiddleWare::sendData(const std::string&& opponent, Task& task) {
  updateClock();
  int socketIndex = _nameToSocketIndex[opponent]; 
  if (socketIndex == 0) {
    fprintf(stderr, "socket is not exist!");
    return false;
  }
  return _sender->sendData(socketIndex, REQ_FLAG, _elapsed, sizeof(int), reinterpret_cast<char*>(&task.priority)); // sizeof(int)... a, spred enzanshi janaidesu
}

bool MiddleWare::terminate(const std::string&& opponent) {
  if (_senderHasSockets == 0) {
    fprintf(stderr, "no sockets exisits\n");
    return false;
  }
  if (!_sender->terminate(_nameToSocketIndex[opponent])) {
    fprintf(stderr, "socket was terminated\n");
    return false;
  }
  _senderHasSockets -= 1;
  _nameToSocketIndex[opponent] = 0;
  fprintf(stderr, "terminate socket is success!");
  return true;
}

void MiddleWare::notify(char *buf = nullptr) {
  std::lock_guard<std::mutex> lock(_mtx);
  // needs Logger
  const short flag = *(short*)buf;
  _logger.showMessage(flag, _elapsed);
  if (buf != nullptr) {
    // assert(buf != nullptr)
    _logger.showData(buf);
  }
}