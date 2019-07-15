#ifndef _INC_RECEIVER_
#define _INC_RECEIVER_ 1

#define SERVER_IP       0x00000000UL

#include "MiddleWare.hpp"
#include "Util.hpp"
#include "RecvThread.hpp"

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
  int _port;
  MiddleWare *_delegator;
  util::Socket _server;
  std::vector<std::unique_ptr<util::Socket>> _sockets;
  std::vector<std::unique_ptr<std::thread>> _threads;
public:
  /* :constructors */
  Receiver(MiddleWare *middleWare, int port) noexcept;
  /* :destructors */
  ~Receiver();

  bool init() noexcept;
  void run() noexcept;
  bool wait();
  bool setSocketOption() noexcept;
  bool startThread(int threadNumber, util::Socket *sock);
  void receiverClose() noexcept;
};

#endif // _INC_RECEIVER_