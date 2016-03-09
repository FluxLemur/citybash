#include <iostream>
#include <string>

#include "city.h"
#include "constants.h"
#include "game_state.h"
#include "utils.h"

GameState::GameState() {
  state_ = PlayState::ADMIN_WAIT;
}

/******************************************************************************/
// Utility Functions
/******************************************************************************/

/**
 * Example: invalid_command("yo", {"no", "help"}) returns:
 * INVALID: [yo]
 * VALID:
 *   [no]
 *   [help]
 */
std::string GameState::invalid_command(std::string command,
    std::vector<std::string> valid_commands) {
  std::string response = "INVALID: [" + command + "]\n";
  response += "VALID:\n";
  for (auto const &valid : valid_commands) {
    response += "  [" + valid + "]\n";
  }

  return response;
}

void GameState::generate_world() {
  // remove any cities with -1 city_id in city_map_
  // call world_.generate() to place cities and start game timer
}

std::string GameState::generate_key() {
  std::string key = "";
  do {
    key = Utils::random_upper_alphanum(KEY_LENGTH);
  } while (city_map_.find(key) != city_map_.end());

  return key;
}


/******************************************************************************/
// Admin Requests
/******************************************************************************/
std::string GameState::PLAYER_JOIN_REQ = "PLAYER_JOIN";
std::string GameState::SERVER_INFO = "SERVER_INFO";
std::string GameState::START_GAME = "START_GAME";
std::string GameState::PLAYERS = "PLAYERS";
std::string GameState::NEW_KEY = "NEW_KEY";
std::string GameState::STATS = "STATS";
std::string GameState::MAP = "MAP";
std::string GameState::FORCE_FINISH = "FORCE_FINISH";
std::string GameState::LEADERBOARD = "LEADERBOARD";
std::string GameState::TERMINATE = "TERMINATE";

std::string GameState::admin_request(std::string command) {
  switch (state_) {
    case ADMIN_WAIT:
      if (command.compare(PLAYER_JOIN_REQ) == 0) {
        return admin_player_join();
      } else if (command.compare(SERVER_INFO) == 0) {
        return admin_server_info();
      } else {
        return invalid_command(command, {PLAYER_JOIN_REQ, SERVER_INFO});
      }
    case PLAYER_JOIN:
      if (command.compare(START_GAME) == 0) {
        return admin_start_game();
      } else if (command.compare(PLAYERS) == 0) {
        return admin_players();
      } else if (command.compare(SERVER_INFO) == 0) {
        return admin_server_info();
      } else if (command.compare(NEW_KEY) == 0) {
        return admin_new_key();
      } else {
        return invalid_command(command,
            {START_GAME, PLAYERS, SERVER_INFO, NEW_KEY});
      }
    case PLAYING:
      if (command.compare(STATS) == 0) {
        return admin_stats();
      } else if (command.compare(MAP) == 0) {
        return admin_map();
      } else if (command.compare(FORCE_FINISH) == 0) {
        return admin_force_finish();
      } else {
        return invalid_command(command, {STATS, MAP, FORCE_FINISH});
      }
    case FINISHED:
      if (command.compare(LEADERBOARD) == 0) {
        return admin_leaderboard();
      } else if (command.compare(TERMINATE) == 0) {
        return admin_terminate();
      } else {
        return invalid_command(command, {LEADERBOARD, TERMINATE});
      }
  }
}

std::string GameState::admin_player_join() {
  std::cout << "** STARTING PLAYER JOIN STATE **\n";
  state_ = PlayState::PLAYER_JOIN;
  return PLAYER_JOIN_REQ + ": SUCCESS\n";
}

std::string GameState::admin_server_info() {
  return SERVER_INFO + ": TODO\n";
}

std::string GameState::admin_start_game() {
  std::cout << "** STARTING GAME **\n";
  generate_world();
  state_ = PlayState::PLAYING;
  return START_GAME + ": SUCCESS\n";
}

std::string GameState::admin_players() {
  std::map<std::string, city_id>::iterator it;
  std::string curr_players = "key city_name\n";
  for (it = city_map_.begin(); it != city_map_.end(); it++) {
    curr_players += it->first + " ";
    curr_players += world_.name_of(it->second) + "\n";
  }
  return curr_players;
}

std::string GameState::admin_new_key() {
  std::string key = generate_key();
  city_map_.insert(
      std::pair<std::string, city_id>(key, City::get_next_city_id()));
  return NEW_KEY + ": " + key + "\n";
}

std::string GameState::admin_stats() {
  return STATS + ": TODO\n";
}

std::string GameState::admin_map() {
  return MAP + ": TODO\n";
}

std::string GameState::admin_force_finish() {
  std::cout << "** ADMIN FORCE GAME FINISH **\n";
  state_ = PlayState::FINISHED;
  return FORCE_FINISH + ": SUCCESS\n";
}

std::string GameState::admin_leaderboard() {
  return LEADERBOARD + ": TODO\n";
}

std::string GameState::admin_terminate() {
  std::cout << "** CityBash TERMINATING... **\n";
  return TERMINATE + ": CityBash terminating...\n";
}


/******************************************************************************/
// Player Requests
/******************************************************************************/

std::string GameState::PLAYER_VALID_COMMANDS =
    "  [player key] WORLD\n"
    "  [player key] CITY\n"
    "  [player key] COSTS\n"
    "  [player key] UPGRADE\n"
    "  [player key] TRAIN [# soldiers]\n"
    "  [player key] ATTACK [city_name] [# soldiers]\n";

city_id GameState::city_id_for_key(std::string player_key) {
  std::map<std::string, city_id>::iterator city_it;
  city_it = city_map_.find(player_key);

  if (city_it == city_map_.end()) {
    return City::INVALID_CITY;
  } else {
    return city_it->second;
  }
}

std::string GameState::player_request(std::vector<std::string> split_req) {
  if (split_req.size() < 2) {
    return Responses::INVALID;
  }

  std::string player_key = split_req[0];
  city_id id = city_id_for_key(player_key);
  std::string command = split_req[1];

  switch (state_) {
    case ADMIN_WAIT:
      return "ERROR: please wait for administrator to start PLAYER JOIN phase\n";
    case PLAYER_JOIN:
      if (split_req.size() != 2) {
        return "ERROR: use valid join syntax: [player key] [city_name_no_spaces]\n";
      } else {
        if (id == City::INVALID_CITY) {
          return "INVALID PLAYER KEY: [" + player_key + "]\n";
        } else {
          return player_join(id, command);
        }
      }
    case PLAYING:
      if (id == City::INVALID_CITY) {
        return "INVALID PLAYER KEY: [" + player_key + "]\n";
      }

      if (command.compare(Requests::WORLD) == 0) {
        return player_world(id);

      } else if (command.compare(Requests::CITY) == 0) {
        return player_city(id);

      } else if (command.compare(Requests::COSTS) == 0) {
        return player_costs(id);

      } else if (command.compare(Requests::UPGRADE) == 0) {
        return player_upgrade(id);

      } else if (command.compare(Requests::TRAIN) == 0) {
        if (split_req.size() < 3 || !Utils::is_number(split_req[2])) {
          return Responses::INVALID_TRAIN;
        } else {
          int num_soldiers = std::stoi(split_req[2], nullptr, 10);
          return player_train(id, num_soldiers);
        }

      } else if (command.compare(Requests::ATTACK) == 0) {
        if (split_req.size() < 4 || !Utils::is_number(split_req[3])) {
          return Responses::INVALID_ATTACK;
        } else {
          int num_soldiers = std::stoi(split_req[3], nullptr, 10);
          return player_attack(id, split_req[2], num_soldiers);
        }
      } else {
        return "INVALID: [" + command + "]\nVALID:\n" + PLAYER_VALID_COMMANDS;
      }
    case FINISHED:
      return "ERROR: game has finished\n";
  }
}

std::string GameState::player_join(city_id id, std::string city_name) {
  switch (world_.add_city(id, city_name)) {
    case World::AddCityResponse::SUCCESS:
      std::cout << id << " has joined as " << city_name << std::endl;
      return "The city of " + city_name + " is welcomed warmly to CityBash!\n";
    case World::AddCityResponse::CITY_EXISTS:
      return "ERROR: your player key already been used.\n";
    case World::AddCityResponse::NAME_EXISTS:
      return "ERROR: " + city_name + " is already taken.\n";
  }
}

std::string GameState::player_world(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::player_city(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::player_costs(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::player_upgrade(city_id id) {
  std::cout << id << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::player_train(city_id id, int soldiers) {
  std::cout << id << soldiers << std::endl;
  return Responses::NOT_IMPLEMENTED;
}

std::string GameState::player_attack(city_id from_city, std::string to_city, int soldiers) {
  std::cout << from_city << to_city << soldiers << std::endl;
  return Responses::NOT_IMPLEMENTED;
}
