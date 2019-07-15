#ifndef _INC_SENDER_
#define _INC_SENDER_ 1

#include "Util.hpp"
#include "MiddleWare.hpp"

#include <vector>
#include <string>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace util;

class MiddleWare;

class Sender {
  /* :members */
  std::vector<std::unique_ptr<Socket>> _sockets;
  MiddleWare *const _delegator;
public:
  /* constructors */
  Sender(MiddleWare *const delegator) noexcept;

  /* methods */
  int AddNewClient(const std::string&& ipaddr, const int port) noexcept;
  int initConnector(const std::string&& ipaddr, const int port) noexcept;
  int ConnectToServer() noexcept;
  int sendData(const int socketIndex, const short packetFlags, const double time, const ::size_t size, void* data) noexcept;
  void constructPacket(const short packetFlags, const double time, const ::size_t size, char* buf, void* data) noexcept;
  bool terminate(const int socketIndex);
  bool terminateSocket(const int socketIndex);
};

#endif // _INC_SENDER_