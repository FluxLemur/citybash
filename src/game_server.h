/**
 * The GameServer is an implementation of a server that uses the Game protocol
 * and keeps track of game state.
 */

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <map>
#include <vector>
#include <string>

#include "game_state.h"
#include "city.h"

class GameServer {
  private:
    GameState game_state_;
    static std::string WELCOME_MESSAGE_;
    static const int BACKLOG_ = 10;
    std::string admin_key_;
    std::string handle_req_(std::string request);

  public:
    GameServer(std::string admin_key);

    /* The server main loop. */
    [[noreturn]] void run();
};

#endif // GAME_SERVER_H
