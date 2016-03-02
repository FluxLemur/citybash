#include <iostream>

#include "constants.h"
#include "game_state.h"

GameState::GameState() {
}

std::string GameState::get_world_info(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::get_city_info(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::get_costs_info(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::upgrade_city(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::train_soldiers(city_id id, int soldiers) {
  std::cout << id << soldiers << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::start_attack(city_id from_city, std::string to_city, int soldiers) {
  std::cout << from_city << to_city << soldiers << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

