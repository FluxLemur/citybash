/**
 * The GameServer is an implementation of a server that uses the Game protocol
 * and keeps track of game state.
 */

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <string>
#include "game_state.h"


#define PORT 12345
#define BACKLOG 10


class GameServer {
  private:
    GameState* game_state_;
    static std::string welcome_;

  public:
    GameServer();

    /* The server main loop. */
    void run();

    /* Handles a client request by calling the GameState.
     * The input may not be according to the server interface, yet the
     * output is.
     *
     * @parameter [request] from the client
     * @returns response to the request
     */
    std::string handle_client_rq(std::string request);
};

#endif // GAME_SERVER_H
