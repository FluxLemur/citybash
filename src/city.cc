#include <string>

#include "city.h"

city_id City::INVALID_CITY = -1;
city_id City::current_id = -1;

city_id City::get_next_city_id() {
  current_id++;
  return current_id;
}

City::City(std::string name) {
  level_ = 1;
  gold_ = 0;
  income_ = 1;
  soldiers_ = 0;
  name_ = name;
}

void City::collect_income() {
}

bool City::upgrade() {
  return true;
}

std::string City::get_name() {
  return name_;
}
