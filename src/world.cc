#include <set>

#include "world.h"

World::World(int width) {
  width_ = width;
}

World::AddCityResponse World::add_city(city_id id, std::string name) {
  std::set<std::string>::iterator names_it;
  names_it = city_names_.find(name);
  if (names_it != city_names_.end()) { // name already exists
    return AddCityResponse::NAME_EXISTS;
  }

  std::map<city_id, City*>::iterator it;
  it = city_map_.find(id);
  if (it != city_map_.end()) { // city already exists
    return AddCityResponse::CITY_EXISTS;
  }

  city_names_.insert(name);
  city_map_.insert(std::pair<city_id, City*>(id, new City(name)));
  return AddCityResponse::SUCCESS;
}

std::string World::name_of(city_id id) {
  std::map<city_id, City*>::iterator it;
  it = city_map_.find(id);
  if (it == city_map_.end()) {
    return "ERROR: invalid city id\n";
  } else {
    return it->second->get_name();
  }
}
