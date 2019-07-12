#include "header/RecvThread.hpp"

#include <iostream>

RecvThread::RecvThread(MiddleWare *middleWare, util::Socket *sock) noexcept : 
_delegator(middleWare), _socket(sock)
{
  recvData();
}

bool RecvThread::recvData() noexcept {
  while (true)  {
    buf.reset(new char[1024]);
    int len;
    if ((len = recv(_socket->sock, buf.get(), 1024, 0)) > 0) {
      notify();
    }
  }
  
  // terminate? 
}

void RecvThread::notify() const noexcept {
  _delegator->notify(buf.get());
}