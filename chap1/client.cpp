#define _DEBUG_ 1
#include <iostream>

#include "client.hpp"

Connector::Connector() noexcept : data(new char[packet_size_max]) {}

Connector::~Connector() noexcept {
  delete[] data;
}

bool Connector::initConnector() noexcept {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket");
    std::terminate();
  }

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip_addr);
  return ConnectToServer();
}

bool Connector::ConnectToServer() const noexcept {
  std::cout << "connect to server" << std::endl;
	if (connect(sock, (SOCK*) &server, sizeof(server))) {
		fprintf(stderr, "connection error\n");
		return false;
	}

  std::cout << "connecting prompt is success!" << std::endl;
	return true;
}

bool Connector::fileOpen() {
  std::ifstream ifs;
  // default in
  ifs.open(this->file_name, std::ios::binary);
  while (!ifs.eof()) {
    std::getline(ifs, file_contents);
  }
  this->file_contents_len = file_contents.size();
  
  return true;
}

bool Connector::sendData() {
  printf("length of sending data: %lu\n", packet_size);
  if (send(sock, (void*)data, 1024, 0) == -1) {
    perror("connector send");
    return false;
  }

  return true;
}

bool Connector::constructPacket() {
  printf("start construct packet\n");
  this->packet_size = 0;
  return addDIP();
}

bool Connector::addDIP() {
  printf("--- dip packet ---\n");

  DIP* dip = (DIP*)data;
  dip->ttl = 10;
  dip->version = 1;
  dip->type = 1;

  printf("ttl %d\n", dip->ttl);
  printf("version %d\n", dip->version);
  printf("type %d\n", dip->type);

  this->packet_size += dip_size;

  return addLinkLayer(dip->type);
}

bool Connector::addLinkLayer(int type) {
  printf("\n--- Link Layer ---\n");
  void* buf = &((DIP*)data)[1];
  switch (type) {
    case 1: {
      DTCP* tcp_buf = (DTCP*)buf;
      MDFile(file_name.c_str(), tcp_buf->t);
      fileOpen();
      printMD5(tcp_buf->t);
      // for (int i = 0; i < )
      tcp_buf->type = 111;
      tcp_buf->len = this->file_contents_len;
      printf("type %d\n", tcp_buf->type);
      printf("len %d\n", tcp_buf->len);

      this->packet_size += dtcp_size;
      break;
    }

    case 0: {
      fileOpen();
      DUDP* udp_buf = (DUDP*)buf;
      udp_buf->type = 111;
      udp_buf->len = this->file_contents_len;
      this->packet_size += dudp_size;
      break;
    }

    default: {
      fprintf(stderr, "error: invalid packet type\n");
      return false;
    }
  }

  return addData(type);
}

bool Connector::addData(int type) {
  printf("\n--- data layer ---\n");
  std::size_t beg;
  switch (type) {
    case 1: { // TCP
      beg = dip_size + dtcp_size;
      break;
    }
    case 0: { // UDP
      beg = dip_size + dudp_size;
      break;
    }
  }
  this->packet_size += file_contents_len;
  
  memcpy((data + beg), file_contents.c_str(), file_contents_len);

  for (int i = 0; i < 1024 - beg; ++i) {
    if (i % 16 == 0) printf("\n");
    printf("%02x ", data[i + beg] & 0xff);
  }
  printf("\n");
  return sendData();
}

void Connector::setContents(std::string&& contents) noexcept {
  this->file_contents = std::move(contents);
}

void Connector::setFilename(std::string&& name) noexcept {
  this->file_name = std::move(name);
}

inline void Connector::printMD5(unsigned char const digests[16]) const {
  printf("md5 hash ");
  for (int i = 0; i < 16; ++i) {
    printf("%02x", digests[i]);
  }
  printf("\n");
}

// ./client packettype filename
int main(int argc, char *argv[]) {
  // printf("%d\n", argc);
  if (argc != 3) {
    fprintf(stderr, "needs 2 params, packet type, file name\n");
    std::abort();
  } 
  
  Connector con;
  std::string name(argv[2]);
  con.setFilename(std::move(name));

  if (!con.initConnector()) {
    fprintf(stderr, "failiar to connect server\n");
    std::abort();
  }
  
#if _DEBUG_
  if (!con.constructPacket()) {
    exit(1);
  }
#else
  con.setContents("this is test\n");
  con.sendData();
#endif

  // end
}