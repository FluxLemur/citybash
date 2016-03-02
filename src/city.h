/*
 * A City has a level, gold, income, and soldiers.
 */

#ifndef CITY_H
#define CITY_H

typedef int city_id;

class City {
  private:
    int level;
    int gold;

    /* in units gold/sec */
    int income;
    int soldiers;

  public:
    City(bool is_human);
    void collect_income();
    bool upgrade();

};

#endif // CITY_H
