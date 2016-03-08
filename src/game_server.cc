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

#define BUFLEN 25

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

  if (split_req.size() < 1) {
    return Responses::INVALID;
  }

  if (split_req[0].compare(admin_key_) == 0) {
    split_req.erase(split_req.begin());
    return handle_admin_req_(split_req);
  } else {
    return handle_client_req_(split_req);
  }
}

/**
 * Finds the corresponding city belonging to the client, and passes on the
 * command (potentially with other arguments) to the game state.
 */
std::string GameServer::handle_client_req_(std::vector<std::string> split_req) {
  if (split_req.size() < 2) {
    return Responses::INVALID;
  }
  std::string city_hash = split_req[0];
  split_req.erase(split_req.begin());
  city_id client_city_id;

  std::map<std::string, city_id>::iterator city_id_it;
  city_id_it = client_city_hashes_.find(city_hash);
  if (city_id_it == client_city_hashes_.end()) {
    return Responses::INVALID_CITY_HASH;
  } else {
    client_city_id = city_id_it->second;
  }

  std::string command = split_req[0];
  split_req.erase(split_req.begin());

  return game_state_.player_request(command, client_city_id, split_req);
}

std::string GameServer::handle_admin_req_(std::vector<std::string> split_req) {
  if (split_req.size() == 0) {
    return Responses::INVALID;
  }

  return game_state_.admin_request(split_req[0]);
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
  if (bind(sockfd, (struct sockaddr*) (&serv_addr), sizeof(serv_addr)) < 0) {
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
    int client = accept(sockfd, (struct sockaddr *) (&client_addr), &client_sock_size);
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

    response = handle_req_(std::string(buffer));

    // give client the response
    if (send(client, response.c_str(), response.size(), 0) < 0) {
      perror("send response");
    }

    close(client);
  }
}
