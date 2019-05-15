#include "server.hpp"

#include <iostream>
#include <cstring>

bool Server::initServer() noexcept {
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(server_ip_addr);
  server.sin_port = htons(server_port);
  std::cout << "init server" << std::endl;

  return openServer();
}

bool Server::openServer() noexcept {
  server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (server_socket == -1) {
    perror("server wait socket");
    std::abort();
  }
  std::cout << "create socket" << std::endl;
  if (bind(server_socket, (SOCK*)&server, sizeof(server)) == -1) {
    perror("server bind");
    std::abort();
  }
  std::cout << "bind socket" << std::endl;
  if (listen(server_socket, 5) == -1) {
		perror("server listen");
		std::abort();
	}
  std::cout << "listen" << std::endl;
  return true;
}

bool Server::wait() noexcept {
  socklen_t client_len = sizeof(client);
  memset(&client, 0, client_len);

  while (true) {
    client_socket = accept(server_socket, (SOCK*) &client, &client_len);
    std::cout << "accept client connection attempt" << std::endl;
    if (client_socket != -1) {
      break;
    } else if (client_socket == -1) {
      return false;
    }
  }
  printf("server open\n");
  return true;
}

bool Server::recvData() noexcept {
  const std::size_t len = recv(client_socket, buf, packet_size_max, 0);
  printf("len -> %lu\n", len);
  if (len == -1) {
    perror("server recv");
    return false;
  }
  return reconstructDIP(len);
}

bool Server::reconstructDIP(std::size_t len) const {
  dip* dip_buf = (dip*)(buf + (len - dip_size));
  printf("--- dip layer ---\n\n");
  printf("ttl %d\n", dip_buf->ttl);
  printf("version %d\n", dip_buf->version);
  printf("type %d\n", dip_buf->type);

  printf("\n");
  return reconstructLinkLayer(len - dip_size, dip_buf->type);
}

bool Server::reconstructLinkLayer(std::size_t cursor, int type) const {
  std::size_t data_len;
  printf("--- link layer ---\n\n");
  switch(type) {
    case 1: { // TCP
      printf("%d\n", cursor + dtcp_size);
      dtcp *dtpc_buf = (dtcp*)(buf + (cursor - dtcp_size));
      printf("md5 hash ");
      for (int i = 0; i < 16; ++i) {
        printf("%02x", dtpc_buf->t[i]);
      }
      printf("\ncheck md5 hash ");
      
      unsigned char p[16];
      
      MDString(buf, p, cursor - dtcp_size);
      for (int i = 0; i < 16; ++i) {
        // printf("%02x", p[i]);
        if (p[i] != dtpc_buf->t[i]) {
          fprintf(stderr, "invalid data buffer\n");
          return false;
        }
      }

      printf("ok.\n");
      
      printf("type %d\n", dtpc_buf->type);
      printf("len %d\n", dtpc_buf->len);
      
      cursor += dtcp_size;
      data_len = dtpc_buf->len;
      break;
    }

    case 0: { // UDP
      dudp *dudp_buf = (dudp*)(buf - (cursor + dudp_size));

      printf("type UDP\n");
      printf("len %d\n", dudp_buf->len);

      cursor += dudp_size;
      data_len = dudp_buf->len;
      break;
    }

    default: {
      printf("unknown packet type\nerror\n");
      return false;
    }
  }

  return reconstructData(data_len);
}

bool Server::reconstructData(std::size_t data_len) const {
  char *data_buf = buf;
  
  printf("\n--- data layer ---\n");

  for (int i = 0; i < data_len; ++i) {
    if (i % 16 == 0) printf("\n");
    printf("%02x ", data_buf[i] & 0xff);
  }
  printf("\n");
  return true;
}

bool Server::run() noexcept {
  
  initServer();
  wait();
  
  recvData();

  return true;
}

int main() {
  Server server;
  server.run();
}

