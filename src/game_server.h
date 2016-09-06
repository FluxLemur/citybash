/**
 * The GameServer is an implementation of a server that uses the Game protocol
 * and keeps track of game state.
 */

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <map>
#include <vector>
#include <string>

#include <event2/event.h>

#include "game_state.h"
#include "city.h"

class GameServer {
  private:
    GameState game_state_;
    static std::string WELCOME_MESSAGE_;
    static const int BACKLOG_ = 15;
    std::string admin_key_;

    // http://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap
    struct CompareSockAddr {
      bool operator()(struct sockaddr sa1, struct sockaddr sa2);
    };
    enum RateLimitResponse {
      PASS, // normal case
      WARN, // first time client exceeds rate limit
      DROP, // during cooldown period after WARN
    };
    static const struct timeval RL_TOKEN_PERIOD;

    static const int RL_MAX_TOKENS = 10;
    static const int RL_COOLDOWN = 10;
    std::map<struct sockaddr, int, CompareSockAddr> client_tokens;

    static void rl_add_token(evutil_socket_t fd, short what, void *arg);
    RateLimitResponse rate_limit_client(struct sockaddr client_addr);

  public:
    GameServer(std::string admin_key);
    std::string handle_req(std::string request);
    static void do_accept(evutil_socket_t listener, short event, void *arg);

    /* The server main loop. */
    void run();
};

#endif // GAME_SERVER_H
