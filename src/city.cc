#include <string>

#include "city.h"

city_id City::INVALID_CITY = -1;
city_id City::current_id = -1;
int City::incomes_[] = {1, 2, 3, 5, 8};
int City::upgrade_costs_[] = {30, 90, 270, 810, 2430};
int City::upgrade_times_[] = {5, 10, 15, 20, 25};
int City::train_time_ = 5;
int City::train_cost_ = 5;

int City::MAX_LEVEL = 5;

city_id City::get_next_city_id() {
  current_id++;
  return current_id;
}

std::string City::city_id_string(city_id id) {
  return std::to_string(id);
}

City::City(std::string name) {
  level_ = 1;
  gold_ = 0.0;
  soldiers_ = 0;
  name_ = name;
}

void City::set_start_time(std::chrono::steady_clock::time_point time) {
  last_income_ = time;
}

void City::update_gold() {
  std::chrono::duration<double> diff = std::chrono::steady_clock::now() - last_income_;
  double delta_sec = diff.count();
  double delta_gold = delta_sec * incomes_[level_ - 1];
  if (delta_gold >= 1) {
    gold_ += delta_gold;
    last_income_ = std::chrono::steady_clock::now();
  }
}

int City::get_gold() {
  update_gold();
  return int(gold_);
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

std::string City::info() {
  std::string info = "";
  info += name_ + "\n";
  info += "  LEVEL  " + std::to_string(level_) + "\n";
  info += "  GOLD   " + std::to_string(get_gold()) + "\n";
  info += "  INCOME " + std::to_string(incomes_[level_ - 1]) + "\n";
  info += "  ARMY   " + std::to_string(soldiers_) + "\n";

  return info;
}

std::string City::costs() {
  std::string costs = "";
  costs += "UPGRADE " + std::to_string(upgrade_costs_[level_ - 1]);
  costs += " " + std::to_string(upgrade_times_[level_ - 1]) + "\n";
  costs += "TRAIN   " + std::to_string(train_cost_);
  costs += " " + std::to_string(train_time_) + "\n";

  return costs;
}

std::string City::upgrade() {
  int gold = get_gold();
  int upgrade_cost = upgrade_costs_[level_ - 1];

  if (level_ == City::MAX_LEVEL) {
    return "UPGRADE FAILURE MAX LEVEL\n";
  }

  if (gold > upgrade_cost) {
    gold_ -= upgrade_cost;
    level_++;
    return "UPGRADE SUCCESS\n";
  }

  std::string response = "UPGRADE FAILURE ";
  response += std::to_string(upgrade_cost) + " > " + std::to_string(gold);
  response += "\n";
  return response;
}

std::string City::train(int num_soldiers) {
  int gold = get_gold();
  int train_cost = num_soldiers * train_cost_;

  if (gold >= train_cost) {
    gold_ -= train_cost;
    soldiers_ += num_soldiers;
    return "TRAIN " + std::to_string(num_soldiers) + " SUCCESS\n";
  }

  std::string response = "TRAIN FAILURE ";
  response += std::to_string(num_soldiers) + " COSTS ";
  response += std::to_string(train_cost) + " > " + std::to_string(gold);
  response += "\n";
  return response;
}
