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

    /*
     * When a client communicates to the server, it prefixes its request with
     * the city hash assigned to it at the start of the game. These hashes map
     * to the id of the city in the game_state. This attempts to enforce
     * authenticy of clients, so a client cannot spoof another city.
     *
     * TODO: Of course this does not prevent Eve from reading the hash, or the
     * contents of any messages going over the network. So, this is a pretty
     * lame start to authenticity, but will be improved...
     */
    std::map<std::string, city_id> client_city_hashes_;

    /* Handles a client request by calling the GameState.
     * Passed in is a sanitized version of the message received by the server.
     */
    std::string handle_client_req_(std::vector<std::string> split_req);

    /* Handles an admin request. These requests are meta-game operations,
     * such as starting the player connection phase, and the actual game start.
     */
    std::string handle_admin_req_(std::vector<std::string> split_req);
    std::string handle_req_(std::string request);

  public:
    GameServer(std::string admin_key);

    /* The server main loop. */
    [[noreturn]] void run();
};

#endif // GAME_SERVER_H
