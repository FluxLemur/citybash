/*
 * A World represents 3D space that has a cities at various locations.
 * It consists of a 2D rectangle, with edges that wrap around, forming
 * a torus.
 */

#ifndef WORLD_H
#define WORLD_H

#include <chrono>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "city.h"
#include "location.h"

class World {
  private:
    int width_;
    std::map<city_id, City*> city_by_id_;
    std::map<std::string, City*> city_names_;
    std::map<Location*, City*> city_locations_;

    void randomly_place_cities();
    void generate_pairwise_distances();
    std::chrono::steady_clock::time_point start_time_;

  public:
    World(int width=50);
    ~World();
    enum AddCityResponse {
      NAME_EXISTS,
      CITY_EXISTS,
      SUCCESS
    };

    /* Attempts to add a city to the world with a given name. If a city with
     * such a name already exists, the city is not added and the function
     * returns false. Otherwise, the city is added to the world, and true is
     * returned.
     */
    AddCityResponse add_city(city_id id, std::string name);

    /* Returns the name of city with given id.
     */
    std::string name_of(city_id id);

    /* Places all cities at random locations in the world and starts the world
     * time.
     */
    void create();

    /* Returns information about other cities, relative to the City
     * corresponding to [id].
     */
    std::string other_cities_info(city_id id);

    /* Returns a string of a given city with its coordinate
     * [city name] [city coordinates]
     */
    std::string city_loc(city_id id);

    std::string all_city_info();

    /* the below city_* functions have a precondition that [id] is contained in
     * city_by_id_ and the corresponding city is initialized
     */
    std::string city_info(city_id id);
    std::string city_costs(city_id id);
    std::string city_upgrade(city_id id);

    /* train and attack have preconditions that soldiers is > 0
     * EXCPET for train, if soldiers is -1, then will use maximum possible #
     */
    std::string city_train(city_id id, int soldiers);
    std::string city_attack(city_id from_city, std::string to_city_name, int soldiers);
};

#endif // WORLD_H
