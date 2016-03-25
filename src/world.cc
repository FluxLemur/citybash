#include <iostream>
#include <set>

#include "location.h"
#include "world.h"

World::World(int width) {
  width_ = width;
}

World::~World() {
  std::map<city_id, City*>::iterator it;
  for (it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    delete it->second;
  }
}

World::AddCityResponse World::add_city(city_id id, std::string name) {
  std::set<std::string>::iterator names_it;
  names_it = city_names_.find(name);
  if (names_it != city_names_.end()) { // name already exists
    return AddCityResponse::NAME_EXISTS;
  }

  std::map<city_id, City*>::iterator it;
  it = city_by_id_.find(id);
  if (it != city_by_id_.end()) { // city already exists
    return AddCityResponse::CITY_EXISTS;
  }

  city_names_.insert(name);
  city_by_id_.insert(std::pair<city_id, City*>(id, new City(name)));
  return AddCityResponse::SUCCESS;
}

std::string World::name_of(city_id id) {
  std::map<city_id, City*>::iterator it;
  it = city_by_id_.find(id);
  if (it == city_by_id_.end()) {
    return "Error: city not yet created";
  } else {
    return it->second->get_name();
  }
}

void World::create() {
  randomly_place_cities();
  generate_pairwise_distances();
  start_time_ = std::chrono::steady_clock::now();

  std::map<city_id, City*>::iterator it;
  for (it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    it->second->set_start_time(start_time_);
  }
}

void World::randomly_place_cities() {
  std::map<city_id, City*>::iterator it;
  for (it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    Location* new_loc;

    do {
      new_loc = Location::random_location(width_);
    } while (city_locations_.find(new_loc) != city_locations_.end());

    city_locations_.insert(std::pair<Location*, City*> (new_loc, it->second));

    it->second->set_location(new_loc);
    std::cout << "generating city " << it->second->get_name();
    std::cout << " at " << *new_loc << std::endl;
  }
}

void World::generate_pairwise_distances() {
  std::map<city_id, City*>::iterator it1;
  std::map<city_id, City*>::iterator it2;
  for (it1 = city_by_id_.begin(); it1 != city_by_id_.end(); it1++) {
    it2 = it1;
    it2++;
    for (; it2 != city_by_id_.end(); it2++) {
      std::cout << "generating distance between " << it1->second->get_name() << " and ";
      std::cout << it2->second->get_name();
      float distance = Location::distance_between(it1->second->get_location(),
                                                   it2->second->get_location());
      std::cout << ": " << distance << std::endl;
      it1->second->add_neighbor(it2->second, distance);
      it2->second->add_neighbor(it1->second, distance);
    }
  }
}

std::string World::other_cities_info(city_id id) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  if (it == city_by_id_.end() || it->second == nullptr) {
    return "ERROR: no city with id " + std::to_string(id) + "\n";
  }

  return it->second->display_all_neighbor_info();
}

std::string World::city_loc(city_id id) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  if (it == city_by_id_.end() || it->second == nullptr) {
    return "ERROR: no city with id " + std::to_string(id) + "\n";
  }

  City &city = *it->second;
  return city.get_location().to_string() + "\t" + city.get_name();
}

std::string World::all_city_info() {
  std::string info_str;
  std::map<city_id, City*>::iterator it;
  for (it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    info_str += it->second->info() + "\n";
  }
  return info_str;
}

std::string World::city_info(city_id id) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  City &city = *it->second;
  return city.info();
}

std::string World::city_costs(city_id id) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  City &city = *it->second;
  return city.costs();
}

std::string World::city_upgrade(city_id id) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  City &city = *it->second;
  return city.upgrade();
}
