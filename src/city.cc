#include <string>

#include "city.h"

city_id City::INVALID_CITY = -1;
city_id City::current_id = -1;

city_id City::get_next_city_id() {
  current_id++;
  return current_id;
}

std::string City::city_id_string(city_id id) {
  return std::to_string(id);
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

void City::set_location(Location* l) {
  loc_ = l;
}

Location City::get_location() {
  return *loc_;
}

void City::add_neighbor(City* neighbor, float distance) {
  neighbors_.insert(std::pair<City*, float>(neighbor, distance));
}

std::string City::display_all_neighbor_info() {
  std::string info_str = "";

  std::map<City*, float>::iterator it;

  for (it = neighbors_.begin(); it != neighbors_.end(); it++) {
    if (it->first) {
      City& other_city = *(it->first);
      float distance_to = it->second;

      // [other name] [other level] [distance from this to other]
      info_str += other_city.get_name() + " ";
      info_str += std::to_string(other_city.level_) + " ";
      info_str += std::to_string(distance_to) + "\n";
    }
  }

  return info_str;
}
