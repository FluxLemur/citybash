#include <iostream>
#include <set>

#include "battle.h"
#include "event_manager.h"
#include "location.h"
#include "utils.h"
#include "world.h"

World::World(int width) {
  width_ = width;
}

World::~World() {
  for (auto it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    delete it->second;
  }
}

World::AddCityResponse World::add_city(city_id id, std::string name) {
  auto names_it = city_names_.find(name);
  if (names_it != city_names_.end()) { // name already exists
    return AddCityResponse::NAME_EXISTS;
  }

  auto it = city_by_id_.find(id);
  if (it != city_by_id_.end()) { // city already exists
    return AddCityResponse::CITY_EXISTS;
  }

  City *new_city = new City(name, id);
  city_names_.insert(std::pair<std::string, City*>(name, new_city));
  city_by_id_.insert(std::pair<city_id, City*>(id, new_city));
  return AddCityResponse::SUCCESS;
}

std::string World::name_of(city_id id) {
  auto it = city_by_id_.find(id);
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

  for (auto it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    it->second->set_start_time(start_time_);
  }
}

void World::randomly_place_cities() {
  for (auto it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    Location* new_loc; // TODO: memory leak

    do {
      new_loc = Location::random_location(width_);
    } while (city_locations_.find(new_loc) != city_locations_.end());

    city_locations_.insert(std::pair<Location*, City*> (new_loc, it->second));

    it->second->set_location(new_loc);
    std::cout << "generating city " << it->second->get_name();
    std::cout << " at " << *new_loc << std::endl;
  }
}

bool World::city_id_exists(city_id id) {
  auto it = city_by_id_.find(id);
  return it != city_by_id_.end();
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
  auto it = city_by_id_.find(id);
  if (it == city_by_id_.end() || it->second == nullptr) {
    return "ERROR: no city with id " + std::to_string(id) + "\n";
  }

  return it->second->display_all_neighbor_info();
}

std::string World::city_loc(city_id id) {
  auto it = city_by_id_.find(id);
  if (it == city_by_id_.end() || it->second == nullptr) {
    return "ERROR: no city with id " + std::to_string(id) + "\n";
  }

  City &city = *it->second;
  return city.get_location().to_string() + "\t" + city.get_name();
}

std::string World::all_city_info() {
  std::string info_str;
  for (auto it = city_by_id_.begin(); it != city_by_id_.end(); it++) {
    info_str += it->second->info(true) + "\n";
  }
  return info_str;
}

std::string World::city_info(city_id id, bool less) {
  auto it = city_by_id_.find(id);
  City &city = *it->second;

  std::string info = city.info(less);
  city.clear_notifications();
  return info;
}

std::string World::city_costs(city_id id) {
  auto it = city_by_id_.find(id);
  City &city = *it->second;
  return city.costs();
}

std::string World::city_upgrade(city_id id) {
  auto it = city_by_id_.find(id);
  City &city = *it->second;
  return city.upgrade();
}

std::string World::city_train(city_id id, int soldiers) {
  auto it = city_by_id_.find(id);
  City &city = *it->second;
  if (soldiers == -1) {
    return city.train_max();
  } else {
    return city.train(soldiers);
  }
}

struct battle_arg {
  City *from_city;
  City *to_city;
  int num_attacking;
  struct event *event_p;
};

struct battle_result_arg {
  City *from_city;
  City *to_city;
  int num_attacking;
  int attackers_remaining;
  int gold_taken;
  int num_defending;
  int defenders_remaining;
  struct event *event_p;
};

void World::battle_result_callback(evutil_socket_t listener, short event, void *arg) {
  (void)(event);    // UNUSED
  (void)(listener); // UNUSED

  struct battle_result_arg *args = (struct battle_result_arg *) arg;
  args->from_city->add_attack_notification(false, args->to_city->get_name(),
      args->num_attacking, args->attackers_remaining, args->gold_taken,
      args->num_defending, args->defenders_remaining);

  args->from_city->change_gold(args->gold_taken);
  args->from_city->change_soldiers(args->attackers_remaining);

  event_free(args->event_p);
  delete args;
}

void World::battle_callback(evutil_socket_t listener, short event, void *arg) {
  (void)(event);    // UNUSED
  (void)(listener); // UNUSED

  struct battle_arg *args = (struct battle_arg *) arg;

  int num_defending = args->to_city->get_soldiers();
  Battle b(args->num_attacking, num_defending, args->to_city->get_level());

  int attacker_capacity = b.attackers_remaining() * 3;
  int gold_taken = args->to_city->change_gold(-attacker_capacity);

  args->to_city->set_soldiers(b.defenders_remaining());

  args->to_city->add_attack_notification(true, args->from_city->get_name(),
      args->num_attacking, b.attackers_remaining(), gold_taken,
      num_defending, b.defenders_remaining());

  // TODO: create event to notifier the attacker
  struct timeval tv;
  tv.tv_sec = int(args->to_city->distance_to(args->from_city));
  tv.tv_usec = 0;

  struct battle_result_arg *battle_result_args = new battle_result_arg;
  battle_result_args->from_city = args->from_city;
  battle_result_args->to_city = args->to_city;
  battle_result_args->num_attacking = args->num_attacking;
  battle_result_args->attackers_remaining = b.attackers_remaining();
  battle_result_args->gold_taken = gold_taken;
  battle_result_args->num_defending = num_defending;
  battle_result_args->defenders_remaining = b.defenders_remaining();

  struct event *battle_result_event;
  battle_result_event = evtimer_new(EventManager::base, battle_result_callback,
      (void *) battle_result_args);
  battle_result_args->event_p = battle_result_event;

  evtimer_add(battle_result_event, &tv);

  event_free(args->event_p);
  delete args;
}

std::string World::city_attack(city_id from_city_id, std::string to_city_str,
    int num_attacking) {

  auto id_it = city_by_id_.find(from_city_id);
  City &from_city = *id_it->second;
  int all_from_soldiers = from_city.get_soldiers();
  if (num_attacking > all_from_soldiers) {
    return "ATTACK FAILURE " + std::to_string(num_attacking) + " > " +
      std::to_string(all_from_soldiers);
  }

  auto name_it = city_names_.find(to_city_str);
  City *to_city;
  if (name_it == city_names_.end()) {

    // try to find the city by ID
    if (Utils::is_number(to_city_str)) {
      city_id to_city_id = city_id(Utils::safe_stoi(to_city_str));
      id_it = city_by_id_.find(to_city_id);

      if (id_it != city_by_id_.end()) {
        to_city = id_it->second;
      } else {
        return "ATTACK FAILURE No city ID " + to_city_str + "\n";
      }
    } else {
      return "ATTACK FAILURE No city " + to_city_str + "\n";
    }
  } else {
    to_city = name_it->second;
  }

  if (to_city->get_name().compare(from_city.get_name()) == 0) {
    return "ATTACK FAILURE Cannot attack your own city\n";
  }

  // remove soldiers from attacking city
  from_city.change_soldiers(-num_attacking);

  // schedule callback to do battle
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  struct battle_arg *battle_args = new battle_arg;
  battle_args->from_city = &from_city;
  battle_args->to_city = to_city;
  battle_args->num_attacking = num_attacking;

  struct event *battle_event;
  battle_event = evtimer_new(EventManager::base, battle_callback, (void *) battle_args);
  battle_args->event_p = battle_event;

  evtimer_add(battle_event, &tv);

  return "ATTACK SENT TO " + to_city->get_name() + " "
         + std::to_string(num_attacking) + "\n";
}
