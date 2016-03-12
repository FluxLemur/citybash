#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "city.h"
#include "constants.h"
#include "game_server.h"
#include "game_state.h"
#include "utils.h"


std::string GameServer::WELCOME_MESSAGE_ = "Welcome to CityBash!\n";

GameServer::GameServer(std::string admin_key) {
  admin_key_ = admin_key;
}

/**
 * Sanitizes a message received by the server.
 * If the first element is the admin key, then handles an admin request.
 * Otherwise, handles input as a client/player request.
 */
std::string GameServer::handle_req_(std::string request) {
  std::vector<std::string> split_req = Utils::upper_trimmed_split(request);

  if (split_req.size() > 1 && split_req[0].compare("CLIENT_HELLO") == 0) {
    return "Hello from CityBash Server\n";
  }

  if (split_req.size() < 2) {
    return "INVALID: [" + Utils::trim(request) + "]\nVALID:\n  [player key] [command]\n";
  }

  if (split_req[0].compare(admin_key_) == 0) {
    return game_state_.admin_request(split_req[1]);
  } else {
    return game_state_.player_request(split_req);
  }
}

static const unsigned int buffer_length = 100;
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
  if (bind(sockfd, (struct sockaddr*) (&serv_addr), sizeof(serv_addr)) < 0) {
    perror("bind");
  }
  if (listen(sockfd, BACKLOG_) < 0) {
    perror("listen");
  }
  std::cout << "CityBash server is running at port " << PORT << std::endl;

  int client_addr;
  socklen_t client_sock_size;
  char buffer[buffer_length];
  std::string response;

  for(;;) {
    client_sock_size = sizeof(client_addr);
    int client = accept(sockfd, (struct sockaddr *) (&client_addr), &client_sock_size);

    // receive a message
    int n = read(client, buffer, buffer_length);
    if (n < 0) {
      perror("read");
    } else if (n == int(buffer_length)) {
      n = int(buffer_length) - 1;
    }
    buffer[n] = '\0';
    std::cout << buffer << std::flush;

    response = handle_req_(std::string(buffer));

    // give client the response
    if (send(client, response.c_str(), response.size(), 0) < 0) {
      perror("send response");
    }

    close(client);
  }
}
