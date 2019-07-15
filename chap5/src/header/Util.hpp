#ifndef _INC_UTIL_
#define _INC_UTIL_ 1

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


namespace util {

  struct Socket {
    int sock;
    struct sockaddr_in sockIn;
    struct sockaddr sockAddr;
  };

  constexpr short REQ_FLAG = 1 << 0;
  constexpr short ACK_FLAG = 1 << 1;
  constexpr short INIT_FLAG = 1 << 2;
  constexpr short TERMINATE_FLAG = 1 << 3;
  constexpr short FUNCTION_EXECTION_FLAG = 1 << 4;

  constexpr short FLAG_SIZE = 2; // byte
  constexpr short TIME_SIZE = 8; // byte
  constexpr int UNIQUE_NUMBER_SIZE = 4; // byte

  constexpr int CLIENT_NUMBER = 2; // size;
}


#endif // _INC_UTIL_