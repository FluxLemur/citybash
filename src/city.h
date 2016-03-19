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
    static int incomes_[];
    static int upgrade_costs_[];
    static int upgrade_times_[];
    static int train_time_;
    static int train_cost_;

    std::string name_;
    int level_;
    int gold_;
    int soldiers_; /* # currently in city */
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

    /* Returns info about the city:
     * [city name]
     * LEVEL [current level]
     * GOLD [current gold]
     * INCOME [current gold income]
     * ARMY [current # of soldiers]
     */
    std::string info();

    /* Gives information about other cities, relative to [this].
     * Sample output:
     * [city1 name] [city1 level] [distance from this to city1]
     * [city2 name] [city2 level] [distance from this to city2]
     * ...
     */
    std::string display_all_neighbor_info();

    /* Returns:
     * UPGRADE [gold to upgrade city] [time to upgrade]
     * TRAIN [gold to train soldier] [time to train]
     */
    std::string costs();
};

#endif // CITY_H
