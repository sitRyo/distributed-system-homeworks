#include "header/RecvThread.hpp"

#include <iostream>
using std::cout;
using std::endl;

RecvThread::RecvThread(MiddleWare *middleWare, util::Socket *sock, const int number) noexcept : 
_delegator(middleWare), _socket(sock), _threadNumber(number)
{
  // printf("thread create\n");
  run();
}

RecvThread::RecvThread(RecvThread&& reTh) :
_delegator(reTh._delegator), _socket(reTh._socket), _threadNumber(reTh._threadNumber)
{}

void RecvThread::run() noexcept {
  recvData();
}

bool RecvThread::recvData() noexcept {
  
  while (true)  {
    buf.reset(new char[1024]);
    int len;
    if ((len = recv(_socket->sock, buf.get(), 1024, 0)) > 0) {
      // cout << "len: "<<  len << endl;
      notify();
    } else {
      perror("recv ");
      return false;
    }
  }
  // terminate? 
}

void RecvThread::notify() const noexcept {
  // cout << "notify" << endl;
  _delegator->notify(_threadNumber, buf.get());
}