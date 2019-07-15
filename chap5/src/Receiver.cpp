#include "header/Receiver.hpp"

#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

Receiver::Receiver(MiddleWare *middleWare, int port) noexcept 
: _delegator(middleWare), _port(port)
{
  _threads.emplace_back(new std::thread()); // dummy
  init();
}

Receiver::~Receiver() {
  for (auto&& th : _threads) {
    if (th->joinable()) {
      th->detach();
    }
  }
}

bool Receiver::init() noexcept {
  // cout << "Receiver::init()" << endl;
  _server.sock = -1;
  _server.sockIn.sin_family = AF_INET;
  _server.sockIn.sin_addr.s_addr = htonl(SERVER_IP);
  _server.sockIn.sin_port = htons(_port);
  _server.sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (!setSocketOption()) { return false; }
  if (bind(_server.sock, (struct sockaddr*)&_server.sockIn, sizeof(_server.sockIn))) {
    perror("Reciver::init bind");
    return false;
  }
  // cout << "bind" << endl;
  if (listen(_server.sock, 5) == -1) {
    perror("Receiver::init listen");
    return false;
  }
  // cout << "listen" << endl;
  return true;
}

bool Receiver::setSocketOption() noexcept {
  if (_server.sock == -1) {
    perror("Receiver::setSocketOption when invoke socket system call");
    return false;
  }
  int flag = 1;
  int ret = setsockopt(_server.sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
  if (ret == -1) {
    perror("Receiver::setSocketOption setsockopt");
    return false;
  }
  return true;
}

void Receiver::run() noexcept {
  while (wait()) {
    startThread(1, _sockets.back().get());
  }
  
  receiverClose();
  // printf("finish\n");
}

void Receiver::receiverClose() noexcept {
  if (_server.sock != -1) {
    close(_server.sock);
    _server.sock = -1;
    // cout << "server close" << endl;
  }
}

bool Receiver::wait() {
  util::Socket *sock = new util::Socket;
  memset(sock, 0, sizeof(*sock));
  sock->sock = -1;
  while (true) {
    socklen_t clientAddrLen = sizeof(sock->sockIn);
    
    sock->sock = accept(_server.sock, (struct sockaddr*)&sock->sockIn, &clientAddrLen);
    cout << sock->sock << endl;
    // cout << "accept" << endl;
    if (sock->sock != -1) {
      // printf("create new connection\n");
      break; 
    }
    if (errno == EINTR)
      continue;
    perror("server open accept");
    return false;
  }
  _sockets.emplace_back(sock);
  return true;
}

bool Receiver::startThread(int threadNumber, util::Socket *sock) {
  _threads.emplace_back(
    new std::thread([this, &sock](auto threadNum) {
      RecvThread newTh(this->_delegator, sock, threadNum);
    }, threadNumber
  ));
  return true;
}