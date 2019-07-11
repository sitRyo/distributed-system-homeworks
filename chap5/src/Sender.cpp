#include "header/Sender.hpp"

#include <iostream>

using namespace util;

Sender::Sender(MiddleWare *const delegator) noexcept
  : _delegator(delegator)
{
  _sockets.emplace_back(new Socket); // dummy
}

int Sender::AddNewClient(const std::string&& ipaddr, const int port) noexcept {
  // assert(_sockets.size() > 0); // MUST!!!!
  return initConnector(std::move(ipaddr), port);
}

int Sender::initConnector(const std::string&& ipaddr, const int port) noexcept {
  Socket* newSock = new Socket;
  newSock->sock = socket(AF_INET, SOCK_STREAM, 0);
  if (newSock->sock == -1) {
    perror("Sender::initConnector create socket discriptor");
    return false;
  }
  newSock->sockIn.sin_family = AF_INET;
  newSock->sockIn.sin_port = htons(port);
  newSock->sockIn.sin_addr.s_addr = inet_addr(ipaddr.c_str());
  _sockets.emplace_back(newSock);
  return ConnectToServer();
}

int Sender::ConnectToServer() noexcept {
  Socket* conSock = _sockets.back().get();
  if (connect(conSock->sock, reinterpret_cast<struct sockaddr*>(&conSock->sockIn), sizeof(conSock->sockIn))) {
    perror("Sender::ConnectToServer connect");
    return false;
  }
  return sendData(_sockets.size() - 1, INIT_FLAG, 0.0, 0, nullptr) ? _sockets.size() - 1 : -1;
}

bool Sender::sendData(const int socketIndex, const short packetFlags, const double time = 0.0, const ::size_t size = 0, void* data = nullptr) noexcept {
  std::unique_ptr<char[]> buf = std::make_unique<char[]>(size + FLAG_SIZE + TIME_SIZE);
  Socket* conSock = _sockets.at(socketIndex).get();
  if (!buf) {
    // peeror("Sender::sendData");
    fprintf(stderr, "Sender::sendData unique_ptr returns nullptr.");
    return false;
  }
  constructPacket(packetFlags, time, size, buf.get(), data);
  if (send(conSock->sock, buf.get(), size + FLAG_SIZE + TIME_SIZE, 0) == -1) {
    perror("Sender::sendData send");
    return false;
  }

  return true;
}

void Sender::constructPacket(const short packetFlags, const double time, const ::size_t size, char* buf, void* data = nullptr) noexcept {
  // assert(size >= sizeof(short) + sizeof(double));
  *reinterpret_cast<short*>(buf) = packetFlags;
  *reinterpret_cast<double*>(&buf[FLAG_SIZE]) = time;
  if (data != nullptr) {
    ::memcpy(&buf[FLAG_SIZE + TIME_SIZE], data, size);
  }
}

bool Sender::terminate(const int socketIndex) {
  try {
    if (_sockets.at(socketIndex)) { 
      return terminateSocket(socketIndex);
    }
  } catch (...) {
    // at returns the exception.
    fprintf(stderr, "socket index is not exitst\n");
  }
  return false;
}

bool Sender::terminateSocket(const int socketIndex) {
  Socket* tSock = _sockets.at(socketIndex).get();
  if (shutdown(tSock->sock, SHUT_WR) == -1) {
    perror("Sender::terminateSocket shutdown");
    return false;
  }
  return true;
}