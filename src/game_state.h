/*
 * GameState maintains the entire state of the game.
 *  i.e. Initialization, actions done in cities, attacks, etc.
 *
 * The game is a state machine with the following states/transisions:
 *  PLAYER_JOIN: players can connect to the game by sending
 *              their unique key (each player can only sign up once) and their
 *              city name. Admin must send [START_GAME] to proceed.
 *  PLAYING: the world is generated and the game commences, players are
 *           allowed to send the regular game commands.
 *  FINISHED: the game is stopped automatically, determined by the winning
 *            conditions as stated in the README.
 *
 * Valid Admin commands for each state are:
 *  PLAYER_JOIN
 *   - START_GAME: generates the world and commences PLAYING state
 *   - KEY: generate a new, unique player key
 *   - PLAYERS: return players who have joined
 *
 *  PLAYING
 *   - INFO: give for each city its gold, gold/sec, level, and army size
 *   - MAP: return a coordinate list of the cities by name
 *   - FORCE_FINISH: forces the game to stop, commencing FINISHED state
 *
 *  FINISHED
 *   - LEADERBOARD: return the player rankings for the game just played
 *   - MAP (as above)
 *
 * Valid Player commands for each (relevant) state are:
 *  PLAYER_JOIN
 *   - [player key] join [city name]
 *
 *  PLAYING
 *   - (see README)
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <string>

#include "city.h"
#include "world.h"

class GameState {
  private:
    enum PlayState {
      PLAYER_JOIN,
      PLAYING,
      FINISHED,
    };
    PlayState state_;
    World world_;
    std::string generate_key();

    /*
     * A player prefixes their request with their player key, assigned by the
     * admin at start of the game. These keys map to their city id. This
     * attempts to enforce authenticy of players, so a player cannot spoof
     * another city.
     * TODO: Consider message encryption
     */
    std::map<std::string, city_id> city_map_;

    std::string invalid_command(std::string command,
        std::vector<std::string> valid_commands);

    static std::string START_GAME;
    static std::string PLAYERS;
    static std::string KEY;
    static std::string INFO;
    static std::string MAP;
    static std::string FORCE_FINISH;
    static std::string LEADERBOARD;

    std::string admin_start_game();
    std::string admin_players();
    std::string admin_key();
    std::string admin_info();
    std::string admin_map();
    std::string admin_force_finish();
    std::string admin_leaderboard();

    static std::string PLAYER_VALID_COMMANDS;
    city_id city_id_for_key(std::string player_key);

    std::string player_join(city_id id, std::string city_name);
    std::string player_world(city_id id);
    std::string player_city(city_id id);
    std::string player_costs(city_id id);
    std::string player_upgrade(city_id id);
    std::string player_train(city_id id, int soldiers);
    std::string player_attack(city_id from_city, std::string to_city, int soldiers);

  public:
    GameState();
    static const int KEY_LENGTH = 10;

    std::string admin_request(std::string command);
    std::string player_request(std::vector<std::string> split_req);

};

#endif // GAME_STATE_H
