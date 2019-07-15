#ifndef _INC_RECVTHREAD_
#define _INC_RECVTHREAD_ 1

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/tcp.h>

#include "MiddleWare.hpp"
#include "Util.hpp"

#include <memory>

class MiddleWare;

class RecvThread {
/* : members */
  MiddleWare *const _delegator;
  util::Socket *_socket;
  std::unique_ptr<char[]> buf;
  int _threadNumber;
public:
/* :constructors */
  RecvThread(MiddleWare *const middleWare, util::Socket *sock, const int number) noexcept;
  RecvThread(RecvThread&& recvThread);
  void run() noexcept;
  bool recvData() noexcept;
  void notify() const noexcept;
};

#endif // _INC_RECVTHREAD_