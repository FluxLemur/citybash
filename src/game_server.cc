#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <event2/event.h>

#include "city.h"
#include "constants.h"
#include "event_manager.h"
#include "game_server.h"
#include "game_state.h"
#include "utils.h"


std::string GameServer::WELCOME_MESSAGE_ = "Welcome to CityBash!\n";
const struct timeval GameServer::RL_TOKEN_PERIOD = {0, 100 * 1000}; // 100 ms
void print_sockaddr(struct sockaddr sa);

GameServer::GameServer(std::string admin_key) {
  admin_key_ = admin_key;
}

/**
 * Sanitizes a message received by the server.
 * If the first element is the admin key, then handles an admin request.
 * Otherwise, handles input as a client/player request.
 */
std::string GameServer::handle_req(std::string request) {
  std::vector<std::string> split_req = Utils::upper_trimmed_split(request);

  if (split_req.size() > 1 && split_req[0].compare("CLIENT_HELLO") == 0) {
    return "Hello from CityBash Server\n";
  }

  if (split_req.size() < 2) {
    return "INVALID: [" + Utils::trim(request) + "]\n" +
      "VALID:\n  [player key] [command]\n";
  }

  if (split_req[0].compare(admin_key_) == 0) {
    return game_state_.admin_request(split_req[1]);
  } else {
    return game_state_.player_request(split_req);
  }
}

bool GameServer::CompareSockAddr::operator() (struct sockaddr sa1,
                                              struct sockaddr sa2) const {
  for (int i = 2; i < 6; ++i) { // TODO: bytes 2,3,4,5 are the ip??
    if (sa1.sa_data[i] < sa2.sa_data[i]) {
      return true;
    }
  }
  return false;
}

void print_sockaddr(struct sockaddr sa) {
  for (int i = 2; i < 6; i++) {
    std::cout << (int) sa.sa_data[i];
    if (i < 5) {
      std::cout << ".";
    }
  }
}

GameServer::RateLimitResponse
GameServer::rate_limit_client(struct sockaddr client_addr) {
  auto search = client_tokens.find(client_addr);

  // localhost is not rate-limited
  if (client_addr.sa_data[2] == 127 && client_addr.sa_data[3] == 0 &&
      client_addr.sa_data[4] == 0 && client_addr.sa_data[5] == 1) {
    return PASS;
  }

  if (search == client_tokens.end()) {
    print_sockaddr(client_addr);
    std::cout << " NEW client" << std::endl;
    client_tokens.insert(std::make_pair(client_addr, RL_MAX_TOKENS - 1));
    return PASS;

  } else {
    if (search->second == 0) {
      std::cout << "WARN client " << search->second << std::endl;
      search->second -= RL_COOLDOWN;
      return WARN;
    } else if (search->second < 0) {
      return DROP;
    } else {
      search->second -= 1;
      return PASS;
    }
  }
}

void GameServer::rl_add_token(evutil_socket_t fd, short what, void *arg) {
  (void)(fd);
  (void)(what);
  GameServer *gs = (GameServer *) arg;

  for (auto client_token = gs->client_tokens.begin();
      client_token != gs->client_tokens.end(); ++client_token) {

    if (client_token->second < RL_MAX_TOKENS) {
      client_token->second += 1;
    }
  }
}

static const unsigned int buffer_length = 100;
void GameServer::do_accept(evutil_socket_t listener, short event, void *arg) {
  (void)(event); // UNUSED

  GameServer *gs = (GameServer *) arg;
  struct sockaddr client_addr;
  socklen_t client_sock_size;
  char buffer[buffer_length];

  client_sock_size = sizeof(client_addr);
  int client = accept(listener, &client_addr,
      &client_sock_size);

  if (gs->rate_limit_client(client_addr) != PASS) {
    close(client);
    return;
  }

  // TODO: consider making the reading, processing, and writing all events
  long n = read(client, buffer, buffer_length);
  if (n < 0) {
    perror("read");
  } else if (n == int(buffer_length)) {
    n = int(buffer_length) - 1;
  }
  buffer[n] = '\0';
  std::cout << buffer << std::flush;

  std::string response = gs->handle_req(std::string(buffer));

  // give client the response
  if (send(client, response.c_str(), response.size(), 0) < 0) {
    perror("send response");
  }

  close(client);
}

void GameServer::run() {
  evutil_socket_t listener;

  EventManager::init();
  struct event_base *base = EventManager::base;
  listener = socket(AF_INET, SOCK_STREAM, 0);

  int enable = 1;
  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  // Send and Recv timeouts on the listener socket
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 10 * 1000; // 10 ms
  setsockopt(listener, SOL_SOCKET, SO_SNDTIMEO, (void *) &tv,
      sizeof(struct timeval));
  setsockopt(listener, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv,
      sizeof(struct timeval));

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(PORT);

  // open and start listening
  if (bind(listener, (struct sockaddr*) (&sin), sizeof(sin)) < 0) {
    perror("bind");
    return;
  }

  if (listen(listener, BACKLOG_) < 0) {
    perror("listen");
    return;
  }

  std::cout << "CityBash server is running at port " << PORT << std::endl;

  struct event *listener_event;
  listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept,
      (void*) this);

  struct event *rl_token_event = event_new(base, -1, EV_PERSIST, rl_add_token,
      (void*) this);

  event_add(listener_event, NULL);
  evtimer_add(rl_token_event, &RL_TOKEN_PERIOD);
  event_base_dispatch(base);
}
