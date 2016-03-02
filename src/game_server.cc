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

GameServer::GameServer(std::string admin_hash) {
  admin_hash_ = admin_hash;
}

std::string GameServer::handle_client_rq(std::string request) {
  std::vector<std::string> split_req = Utils::upper_trimmed_split(request);

  if (split_req.size() < 2) {
    return Responses::INVALID;
  }

  std::string city_hash = split_req[0];
  city_id client_city_id;

  std::map<std::string, city_id>::iterator city_id_it;
  city_id_it = client_city_hashes_.find(city_hash);
  if (city_id_it == client_city_hashes_.end()) {
    return Responses::INVALID_CITY_HASH;
  } else {
    client_city_id = city_id_it->second;
  }

  std::string commmand = split_req[1];

  if (commmand.compare(Requests::WORLD) == 0) {
    return game_state_.get_world_info(client_city_id);

  } else if (commmand.compare(Requests::CITY) == 0) {
    return game_state_.get_city_info(client_city_id);

  } else if (commmand.compare(Requests::COSTS) == 0) {
    return game_state_.get_costs_info(client_city_id);

  } else if (commmand.compare(Requests::UPGRADE) == 0) {
    return game_state_.upgrade_city(client_city_id);

  } else if (commmand.compare(Requests::TRAIN) == 0) {
    if (split_req.size() < 3 || !Utils::is_number(split_req[2])) {
      return Responses::INVALID_TRAIN;
    } else {
      int num_soldiers = std::stoi(split_req[2], nullptr, 10);
      return game_state_.train_soldiers(client_city_id, num_soldiers);
    }

  } else if (commmand.compare(Requests::ATTACK) == 0) {
    if (split_req.size() < 4 || !Utils::is_number(split_req[3])) {
      return Responses::INVALID_ATTACK;
    } else {
      int num_soldiers = std::stoi(split_req[3], nullptr, 10);
      return game_state_.start_attack(client_city_id, split_req[2], num_soldiers);
    }
  }

  return Responses::INVALID;
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

    response = handle_client_rq(std::string(buffer));

    // give client the response
    if (send(client, response.c_str(), response.size(), 0) < 0) {
      perror("send response");
    }

    close(client);
  }
}
