/*
 * A City has a level, gold, income, and soldiers.
 */

#ifndef CITY_H
#define CITY_H

#include <event2/event.h>
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "location.h"

typedef int city_id;

class City {
  private:
    static city_id current_id;
    static int cache_[];
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

    std::vector<std::pair<std::chrono::steady_clock::time_point, std::string>> notifications_;

    /* updates the amount of gold the city has, depending on what the current
     * income is, the time since last call to update_gold()
     */
    void update_gold();
    std::chrono::steady_clock::time_point last_income_;

    /* Map of neighbors to their distance from this City */
    std::map<City*, float> neighbors_;

  public:
    static double defense_multiplier[];
    static city_id INVALID_CITY;
    static city_id get_next_city_id();
    static std::string city_id_string(city_id id);
    static int MAX_LEVEL;

    /* Scheduled functions */
    static void train_callback(evutil_socket_t listener, short event, void *arg);

    City(std::string name);
    std::string get_name();
    void set_location(Location* l);
    Location get_location();
    void add_neighbor(City* neighbor, float distance);
    void set_start_time(std::chrono::steady_clock::time_point time);
    int get_gold();
    int get_level();

    /* Notification related */
    void clear_notifications();
    std::string notification_to_string(std::chrono::steady_clock::time_point& time,
      std::string& contents);
    void add_attack_notification(std::string attacker_city, int n_attackers,
        int n_attackers_remaining, int gold_stolen, int n_defenders,
        int n_defender_remaining);

    /* Changes the gold in this city by [delta], such that [gold_] is not
     * less than 0.
     * Returns the difference between [gold_] before and after
     * If [cache], the city saves prevents the change to drop below the amount
     * of gold the city can cache.
     */
    int change_gold(int delta, bool cache);

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
     *   UPGRADE SUCCESS
     * else returns:
     *   UPGRADE FAILURE [gold needed] > [current gold]
     */
    std::string upgrade();

    /* Precondition: num_soldiers > 0
     * Returns:
     * if [current gold] >= [train_cost_ * num_soldiers] returns:
     *   TRAIN [num_soldiers] SUCCESS
     * else returns:
     *   TRAIN FAILURE [# soldiers] COSTS [gold needed] > [current gold]
     */
    std::string train(int num_soldiers);
    std::string train_max();

    int get_soldiers();
    void set_soldiers(int n);
    void add_soldiers(int n);
};

#endif // CITY_H
