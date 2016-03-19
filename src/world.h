/*
 * A World represents 3D space that has a cities at various locations.
 * It consists of a 2D rectangle, with edges that wrap around, forming
 * a torus.
 */

#ifndef WORLD_H
#define WORLD_H

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
    std::set<std::string> city_names_;
    std::map<Location*, City*> city_locations_;

    void randomly_place_cities();
    void generate_pairwise_distances();

  public:
    World(int width=100);
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
};

#endif // WORLD_H
