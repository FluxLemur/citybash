#include <iostream>
#include <set>

#include "location.h"
#include "world.h"

World::World(int width) {
  width_ = width;
}

World::~World() {
  std::map<city_id, City*>::iterator it;
  for (it = city_map_.begin(); it != city_map_.end(); it++) {
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
    return "Error: city not yet created\n";
  } else {
    return it->second->get_name();
  }
}

void World::create() {
  randomly_place_cities();
  generate_pairwise_distances();
}

void World::randomly_place_cities() {
  std::map<city_id, City*>::iterator it;
  for (it = city_map_.begin(); it != city_map_.end(); it++) {
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
  for (it1 = city_map_.begin(); it1 != city_map_.end(); it1++) {
    it2 = it1;
    it2++;
    for (; it2 != city_map_.end(); it2++) {
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
  std::map<city_id, City*>::iterator it = city_map_.find(id);
  if (it == city_map_.end() || it->second == nullptr) {
    return "ERROR: cannot retrieve information about other cities\n";
  } else {
    return it->second->display_all_neighbor_info();
  }
}