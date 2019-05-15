#ifndef _SERVER_
#define _SERVER_

#include "header.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <array>

class Server {
  using SOCK_IN = struct sockaddr_in;
  using SOCK = struct sockaddr;
private:
  int server_socket;
  int client_socket;
  SOCK_IN server;
  SOCK_IN client;
  static constexpr unsigned long server_ip_addr = 0x00000000UL;
  static constexpr int server_port = 22222;

  char *buf;

  bool initServer() noexcept;
  bool openServer() noexcept;
  bool wait() noexcept;
  bool recvData() noexcept;

  bool reconstructDIP(std::size_t) const;
  bool reconstructLinkLayer(std::size_t, int) const;
  bool reconstructData(std::size_t) const;
public:
  Server(): buf(new char[packet_size_max]) {};
  ~Server() { delete [] buf; }
  
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;

  Server(Server&&) = delete;
  Server& operator=(Server&&) = delete;
  
  bool run() noexcept;
};

#endif