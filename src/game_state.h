/*
 * GameState maintains the entire state of the game.
 *  i.e. Initialization, actions done in cities, attacks, etc.
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "world.h"

typedef int city_id;

class GameState {
  private:
    World world;

  public:
    GameState();

    /* Returns the ID of the created city, or -1 if creation fails. */
    city_id add_random_city();

    bool upgrade_city(city_id id);
    bool train_soldiers(city_id id, int num_soldiers);
    bool start_attack(city_id from_city, city_id to_city);
    bool do_attack(int soldiers, city_id defending_city);
};


#endif // GAME_STATE_H
