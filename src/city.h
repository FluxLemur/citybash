/*
 * A City has a level, gold, income, and soldiers.
 */

#ifndef CITY_H
#define CITY_H

#include <string>

typedef int city_id;

class City {
  private:
    static city_id current_id;

    std::string name_;
    int level_;
    int gold_;
    int income_; /* units: gold/sec */
    int soldiers_;

  public:
    static city_id INVALID_CITY;
    static city_id get_next_city_id();

    City(std::string name);
    void collect_income();
    bool upgrade();
    std::string get_name();
};

#endif // CITY_H
