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

    City(std::string name);
    void collect_income();
    bool upgrade();
    std::string get_name();
    void set_location(Location* l);
    Location get_location();
    void add_neighbor(City* neighbor, float distance);
    std::string display_neighbors();
};

#endif // CITY_H
