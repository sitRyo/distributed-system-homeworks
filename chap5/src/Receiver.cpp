#include "header/Receiver.hpp"

#include <iostream>

Receiver::Receiver(MiddleWare *middleWare) noexcept 
: _delegator(middleWare) 
{}

bool Receiver::init() noexcept {
  _server.sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (!setSocketOption()) { return false; }
  if (bind(_server.sock, (struct sockaddr*)&_server.sockIn, sizeof(_server.sockIn))) {
    perror("Reciver::init bind");
    return false;
  }
  if (listen(_server.sock, 5) == -1) {
    perror("Receiver::init listen");
    return false;
  }
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
    
  }
}

bool Receiver::wait() {
  util::Socket *sock = new util::Socket;
  memset(sock, 0, sizeof(*sock));
  sock->sock = -1;
  while (true) {
    socklen_t clientAddrLen = sizeof(sock->sockIn);
    sock->sock = accept(_server.sock, (struct sockaddr*)&sock->sockIn, &clientAddrLen);
    if (sock->sock != -1) {
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