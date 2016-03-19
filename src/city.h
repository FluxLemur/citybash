/*
 * A City has a level, gold, income, and soldiers.
 */

#ifndef CITY_H
#define CITY_H

#include <map>
#include <string>

#include "location.h"

typedef int city_id;

class City {
  private:
    static city_id current_id;

    std::string name_;
    int level_;
    int gold_;
    int income_; /* units: gold/sec */
    int soldiers_;
    Location* loc_;

    /* Map of neighbors to their distance from this City */
    std::map<City*, float> neighbors_;

  public:
    static city_id INVALID_CITY;
    static city_id get_next_city_id();
    static std::string city_id_string(city_id id);

    City(std::string name);
    void collect_income();
    bool upgrade();
    std::string get_name();
    void set_location(Location* l);
    Location get_location();
    void add_neighbor(City* neighbor, float distance);

    /* Gives information about other cities, relative to [this].
     * Sample output:
     * [city1 name] [city1 level] [distance from this to city1]
     * [city2 name] [city2 level] [distance from this to city2]
     * ...
     */
    std::string display_all_neighbor_info();
};

#endif // CITY_H
