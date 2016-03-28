#include <iostream>
#include <set>

#include "battle.h"
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
  std::map<std::string, City*>::iterator names_it;
  names_it = city_names_.find(name);
  if (names_it != city_names_.end()) { // name already exists
    return AddCityResponse::NAME_EXISTS;
  }

  std::map<city_id, City*>::iterator it;
  it = city_by_id_.find(id);
  if (it != city_by_id_.end()) { // city already exists
    return AddCityResponse::CITY_EXISTS;
  }

  City *new_city = new City(name);
  city_names_.insert(std::pair<std::string, City*>(name, new_city));
  city_by_id_.insert(std::pair<city_id, City*>(id, new_city));
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

  std::string info = city.info();
  city.clear_notifications();
  return info;
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

std::string World::city_train(city_id id, int soldiers) {
  std::map<city_id, City*>::iterator it = city_by_id_.find(id);
  City &city = *it->second;
  return city.train(soldiers);
}

std::string World::city_attack(city_id from_city, std::string to_city_name,
    int num_attacking) {

  std::map<city_id, City*>::iterator id_it = city_by_id_.find(from_city);
  City &attack_city = *id_it->second;
  int all_from_soldiers = attack_city.get_soldiers();
  if (num_attacking > all_from_soldiers) {
    return "ATTACK FAILURE " + std::to_string(num_attacking) + " > " +
      std::to_string(all_from_soldiers);
  }

  std::map<std::string, City*>::iterator name_it = city_names_.find(to_city_name);
  if (name_it == city_names_.end()) {
    return "ATTACK INVALID No city " + to_city_name + "\n";
  }
  City &to_city = *name_it->second;
  int num_defending = to_city.get_soldiers();
  if (to_city.get_name().compare(attack_city.get_name()) == 0) {
    return "ATTACK FAILURE Cannto attack your own city\n";
  }

  Battle b(num_attacking, num_defending, to_city.get_level());

  int attacker_capacity = b.attackers_remaining() * 2;
  int gold_taken = to_city.change_gold(-attacker_capacity);

  attack_city.set_soldiers(all_from_soldiers - num_attacking + b.attackers_remaining());
  attack_city.change_gold(gold_taken);
  to_city.set_soldiers(b.defenders_remaining());

  std::string result = "ATTACK ";
  if (b.attackers_win()) {
    result += "WIN ";
  } else {
    result += "LOSE ";
  }

  result += std::to_string(b.attackers_remaining()) + "/";
  result += std::to_string(num_attacking) + " return with ";
  result += std::to_string(gold_taken) + " gold";
  if (b.attackers_win()) {
    result += ", " + to_city.get_name() + " remaining: ";
    result += std::to_string(b.defenders_remaining()) + "/";
    result += std::to_string(num_defending);
  }

  result += "\n";
  return result;
}
