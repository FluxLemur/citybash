/*
 * A City has a level, gold, income, and soldiers.
 */

#ifndef CITY_H
#define CITY_H

#include <chrono>
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
    double gold_;
    int soldiers_; /* # currently in city */
    Location* loc_;

    int get_gold();

    /* updates the amount of gold the city has, depending on what the current
     * income is, the time since last call to update_gold()
     */
    void update_gold();
    std::chrono::steady_clock::time_point last_income_;

    /* Map of neighbors to their distance from this City */
    std::map<City*, float> neighbors_;

  public:
    static city_id INVALID_CITY;
    static city_id get_next_city_id();
    static std::string city_id_string(city_id id);
    static int MAX_LEVEL;

    City(std::string name);
    std::string get_name();
    void set_location(Location* l);
    Location get_location();
    void add_neighbor(City* neighbor, float distance);
    void set_start_time(std::chrono::steady_clock::time_point time);

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

    /* Returns:
     * if [current gold] >= [upgrade gold] returns:
     *   UPGRADE SUCCES
     * else returns:
     *   UPGRADE FAILURE [# current gold] < [gold needed]
     */
    std::string upgrade();
};

#endif // CITY_H
