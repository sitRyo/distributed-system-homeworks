#ifndef _INC_RECEIVER_
#define _INC_RECEIVER_ 1

#include "MiddleWare.hpp"
#include "Util.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/tcp.h>

#include <errno.h>

#include <string>
#include <memory>
#include <thread>
#include <memory>
#include <vector>

class MiddleWare;

// todo multithread

class Receiver {
  /* :member */
  MiddleWare *_delegator;
  util::Socket _server;
  std::vector<std::unique_ptr<util::Socket>> _sockets;
public:
  /* :constructors */
  Receiver(MiddleWare *middleWare) noexcept;
  /* :destructors */
  ~Receiver() = default;

  bool init() noexcept;
  void run() noexcept;
  bool wait();
  bool setSocketOption() noexcept;
  bool startThread();
};

#endif // _INC_RECEIVER_