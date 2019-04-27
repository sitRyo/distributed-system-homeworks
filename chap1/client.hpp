#ifndef _CLIENT_
#define _CLIENT_

#include "header.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <memory>
#include <fstream>
#include <string>

class Connector {
  using SOCK_IN = struct sockaddr_in;
  using SOCK = struct sockaddr;
private:
  SOCK_IN server;
  int sock;
  static constexpr int port = 22222;
  static constexpr char* ip_addr = "127.0.0.1";

  std::string file_contents;
  std::string file_name;
  std::size_t file_contents_len;
  std::size_t packet_size;
  // use raw pointer
  char* data;
  
  bool ConnectToServer() const noexcept;
 
  bool addDIP();
  bool addLinkLayer(int);
  bool addData(int);
  inline void printMD5(unsigned char const digests[16]) const;
public:
  Connector() noexcept;
  ~Connector() noexcept;
  bool initConnector() noexcept;
  bool fileOpen();
  bool sendData();

  bool constructPacket();

  void setFilename(std::string&&) noexcept;
  void setContents(std::string&&) noexcept; // debug
};

#endif