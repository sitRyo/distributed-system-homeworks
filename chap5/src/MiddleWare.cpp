#include "header/MiddleWare.hpp"
#include <iostream>
#include <type_traits>

using std::cout;
using std::endl;

MiddleWare::MiddleWare(const int uniqueNumber) noexcept {
  _start = std::chrono::system_clock::now();
  _elapsed = 0.0;
  _sender.reset(new Sender(this));
  _receiver.reset(nullptr);
  _senderHasSockets = 0;
  _uniqueNumber = uniqueNumber;

  _tasks.push_back(1);
  _tasks.push_back(2);
};

MiddleWare::~MiddleWare() {
  // cout << "MiddleWare ~MiddleWare" << endl;
  if (_receiver->joinable()) {
    _receiver->join();
  }
  // cout << "All Finished" << endl;
}

void MiddleWare::updateClock() noexcept {
  const std::chrono::system_clock::time_point end 
    = std::chrono::system_clock::now();
  _elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-_start).count();
}

bool MiddleWare::AddNewClient(const int num, const std::string opponent, const std::string&& ipaddr, const int port) noexcept {
  if (!_receiver) { 
    _receiver.reset(
      new std::thread([this, &port]() {
        Receiver rec(this, port);
        rec.run();
      })
    );
  }
  _uniqueNumberToString[num] = opponent;
  std::this_thread::sleep_for(std::chrono::seconds(3)); // wait 
  int index = _sender->AddNewClient(std::move(ipaddr), port);
  if (index != -1) {
    _nameToSocketIndex[opponent] = index;
    return true;
  } else {
    return false;
  }
}

void MiddleWare::showSocketTable() const {
  // std::cout << "----show connecting sockets----" << std::endl;
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

bool MiddleWare::sendAck(const std::string opponent) {
  updateClock();
  // cout << opponent << endl;
  int socketIndex = _nameToSocketIndex[opponent];
  // cout << "socket " << socketIndex << endl;
  return _sender->sendData(socketIndex, ACK_FLAG, _elapsed, sizeof(int), nullptr);
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

void MiddleWare::notify(int threadNumber, char *buf = nullptr) {
  std::lock_guard<std::mutex> lock(_mtx);
  // needs Logger
  const short flag = *(short*)buf;
  int uniq = *reinterpret_cast<int*>(&buf[2]);
  // cout << "uniq" << " " << uniq << endl;
  if (flag != util::INIT_FLAG) _logger.showMessage(flag, _elapsed, uniq);
  if (flag == util::REQ_FLAG) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait 
    sendAck(uniqueToString(uniq));
  } else if (flag == util::ACK_FLAG) {
    countAckNum();
  }
}

int MiddleWare::getUniqueNumber() const noexcept {
  return _uniqueNumber;
}

std::string MiddleWare::uniqueToString(const int unique) {
  return _uniqueNumberToString[unique];
}

bool MiddleWare::countAckNum() noexcept {
  _countAckNum += 1;
  if (_countAckNum == util::CLIENT_NUMBER) {
    executeTasks();
    return true;
  }
  return false;
}

void MiddleWare::executeTasks() noexcept {
  _countAckNum = 0;
  for (auto&& itr : _tasks) {
    cout << "Exection: " << itr << endl;
  }
}
