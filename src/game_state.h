/*
 * GameState maintains the entire state of the game.
 *  i.e. Initialization, actions done in cities, attacks, etc.
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <string>

#include "city.h"
#include "world.h"

class GameState {
  private:
    World world;

    /* The game state associates each city with a city_id */
    std::map<city_id, City> city_map_;

  public:
    GameState();

    std::string get_world_info(city_id id);
    std::string get_city_info(city_id id);
    std::string get_costs_info(city_id id);
    std::string upgrade_city(city_id id);
    std::string train_soldiers(city_id id, int soldiers);
    std::string start_attack(city_id from_city, std::string to_city, int soldiers);
};


#endif // GAME_STATE_H
