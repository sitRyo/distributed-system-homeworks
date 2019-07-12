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

class RecvThread {
/* : members */
  MiddleWare *_delegator;
  util::Socket *_socket;
  std::unique_ptr<char[]> buf;
public:
/* :constructors */
  RecvThread(MiddleWare *middleWare, util::Socket *sock) noexcept;
  bool recvData() noexcept;
  void notify() const noexcept;
};

#endif // _INC_RECVTHREAD_