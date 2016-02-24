#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "constants.h"
#include "game_server.h"
#include "utils.h"

#define BUFLEN 25

std::string GameServer::WELCOME_MESSAGE_ = "Welcome to CityBash!\n";

GameServer::GameServer() {
  game_state_ = new GameState;
}


std::string GameServer::handle_client_rq(std::string request) {
  Utils::upper(request);
  std::vector<std::string> split_req = Utils::split(Utils::trim(request), ' ');

  if (split_req.size() == 0) {
    return Responses::INVALID;
  }

  return split_req[0];
}

void GameServer::run() {
  // initialize and configure the server's one socket
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
  // TODO: in order to enforce timeouts, use SO_SNDTIMEO, SO_RCVTIMEO

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  // open and start listening
  if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
  }
  if (listen(sockfd, BACKLOG_) < 0) {
    perror("listen");
  }
  std::cout << "CityBash server is running at port " << PORT << std::endl;

  int client_addr;
	socklen_t client_sock_size;
  char buffer[25];
  std::string response;

  for(;;) {
    std::cout << "waiting for client..." << std::endl;

    client_sock_size = sizeof(client_addr);
    int client = accept(sockfd, (struct sockaddr *) &client_addr, &client_sock_size);
    if (send(client, WELCOME_MESSAGE_.c_str(), WELCOME_MESSAGE_.size(), 0) < 0) {
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

    response = handle_client_rq(std::string(buffer));

    // give client the response
    if (send(client, response.c_str(), response.size(), 0) < 0) {
      perror("send response");
    }

    close(client);
  }
}
