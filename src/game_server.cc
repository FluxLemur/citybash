#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "game_server.h"

#define BUFLEN 25

std::string GameServer::welcome_ = "Welcome to CityBash!\n";

GameServer::GameServer() {
  game_state_ = new GameState;
}

void GameServer::run() {
  // addresses and other declarations
  int sockfd, client_addr;
  struct sockaddr_in serv_addr;
	socklen_t client_sock_size;
  char buffer[25];

  // configure the socket file descriptor
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  // TODO: in order to enforce timeouts, use SO_SNDTIMEO, SO_RCVTIMEO
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
  }
  if (listen(sockfd, BACKLOG) < 0) {
    perror("listen");
  }

  std::cout << "CityBash server is running at port " << PORT << std::endl;

  for(;;) {
    std::cout << "waiting for client..." << std::endl;
    client_sock_size = sizeof(client_addr);
    int client = accept(sockfd, (struct sockaddr *) &client_addr, &client_sock_size);
    if (send(client, welcome_.c_str(), welcome_.size(), 0) < 0) {
      perror("send welcome");
    }

    // receive a message
    int n = read(client, buffer, BUFLEN);
    if (n < 0) {
      perror("read");
    } else if (n == BUFLEN) {
      n = BUFLEN - 1;
    }
    buffer[n] = '\0';
    std::cout << buffer;

    // TODO: depending on message, make a call to game_state
    // sanitize string, pass to game_state

    // give client the response
    if (send(client, buffer, n, 0) < 0) {
      perror("send repeat message");
    }

    close(client);
  }
}
