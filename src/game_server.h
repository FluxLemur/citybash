/**
 * The GameServer is an implementation of a server that uses the Game protocol
 * and keeps track of game state.
 */

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <map>
#include <string>

#include "game_state.h"
#include "city.h"

class GameServer {
  private:
    GameState game_state_;
    static std::string WELCOME_MESSAGE_;
    static const int BACKLOG_ = 10;
    std::string admin_hash_;

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
     * The input may not be according to the server interface, yet the
     * output is.
     *
     * @parameter [request] from the client
     * @returns response to the request
     */
    std::string handle_client_rq(std::string request);

  public:
    GameServer(std::string admin_hash);

    /* The server main loop. */
    [[noreturn]] void run();
};

#endif // GAME_SERVER_H
