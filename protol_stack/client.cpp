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
  if (send(sock, (void*)data, packet_size, 0) == -1) {
    perror("connector send");
    return false;
  }

  return true;
}

bool Connector::constructPacket() {
  printf("start construct packet\n");
  this->packet_size = 0;
  return addData();
}

bool Connector::addDIP(std::size_t link_len) {
  printf("\n--- dip packet ---\n\n");
  
  DIP* dip = (DIP*)(data + (this->file_contents_len + link_len));
  dip->ttl = 10;
  dip->version = 1;
  dip->type = this->protocol_type;

  printf("ttl %d\n", dip->ttl);
  printf("version %d\n", dip->version);
  printf("type %d\n", dip->type);

  this->packet_size += dip_size;

  return sendData();
}

bool Connector::addLinkLayer() {
  printf("\n--- Link Layer ---\n\n");
  void* buf = data + this->file_contents_len;
  switch (this->protocol_type) {
    case 1: {
      DTCP* tcp_buf = (DTCP*)buf;
      MDFile(file_name.c_str(), tcp_buf->t);
      printMD5(tcp_buf->t);
      
      tcp_buf->type = 111;
      tcp_buf->len = this->file_contents_len;
      printf("type %d\n", tcp_buf->type);
      printf("len %d\n", tcp_buf->len);

      this->packet_size += dtcp_size;
      return addDIP(dtcp_size);
    }

    case 0: {
      DUDP* udp_buf = (DUDP*)buf;
      udp_buf->type = 111;
      udp_buf->len = this->file_contents_len;
      this->packet_size += dudp_size;
      return addDIP(dudp_size);
    }

    default: {
      fprintf(stderr, "error: invalid packet type\n");
      return false;
    }
  }
}

bool Connector::addData() {
  printf("\n--- data layer ---\n");
  fileOpen();
  this->file_contents_len = file_contents.size();
  packet_size += this->file_contents_len;
  
  memcpy(data, file_contents.c_str(), file_contents_len);

  for (int i = 0; i < file_contents_len; ++i) {
    if (i % 16 == 0) printf("\n");
    printf("%02x ", data[i] & 0xff);
  }
  printf("\n");
  return addLinkLayer();
}

void Connector::setContents(std::string&& contents) noexcept {
  this->file_contents = std::move(contents);
}

void Connector::setFilename(std::string&& name) noexcept {
  this->file_name = std::move(name);
}

bool Connector::checkPacketType(const char *user_input_protocol_type) {
  if (!strcmp("TCP", user_input_protocol_type)) {
    this->protocol_type = 1;
    printf("protocol type -> TCP\n");
    return true;
  } else

  if (!strcmp("UDP", user_input_protocol_type)) {
    this->protocol_type = 0;
    printf("protocol type -> UDP\n");
    return true;
  }

  printf("unenable protocol type\n");
  return false;
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
    exit(1);
  } 
  
  Connector con;
  std::string name(argv[2]);
  con.setFilename(std::move(name));
  if (!con.checkPacketType(argv[1])) {
    fprintf(stderr, "please select TCP or UDP (please input uppercase latter)\n");
    con.~Connector();
    exit(1);
  }

  if (!con.initConnector()) {
    fprintf(stderr, "failiar to connect server\n");
    con.~Connector();
    exit(1);
  }
  
#if _DEBUG_
  if (!con.constructPacket()) {
    con.~Connector();
    exit(1);
  }
#else
  con.setContents("this is test\n");
  con.sendData();
#endif

  // end
}