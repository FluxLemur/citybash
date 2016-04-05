#include <iostream>
#include <string>

#include "city.h"
#include "constants.h"
#include "event_manager.h"
#include "game_state.h"
#include "utils.h"

GameState::GameState() {
  state_ = PlayState::PLAYER_JOIN;
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

std::string GameState::generate_key() {
  std::string key = "";
  do {
    key = Utils::random_upper_alphanum(KEY_LENGTH);
  } while (city_map_.find(key) != city_map_.end());

  return key;
}

World &GameState::get_world() {
  return world_;
}

void GameState::finish_state() {
  state_ = PlayState::FINISHED;
}

void GameState::end_game(evutil_socket_t listener, short event, void *arg) {
  (void)(listener); // UNUSED
  (void)(event);    // UNUSED

  GameState * gs = (GameState *) arg;
  std::cout << "GAME OVER!" << std::endl;

  gs->finish_info = gs->get_world().all_city_info();
  gs->finish_condition = "GAME OVER: " + City::winning_city->get_name() + " has WON\n";
  gs->finish_state();
}

/******************************************************************************/
// Admin Requests
/******************************************************************************/
std::string GameState::START_GAME = "START_GAME";
std::string GameState::PLAYERS = "PLAYERS";
std::string GameState::KEY = "KEY";
std::string GameState::INFO = "INFO";
std::string GameState::MAP = "MAP";
std::string GameState::FORCE_FINISH = "FORCE_FINISH";
std::string GameState::LEADERBOARD = "LEADERBOARD";

std::string GameState::admin_request(std::string command) {
  switch (state_) {
    case PLAYER_JOIN:
      if (command.compare(START_GAME) == 0) {
        return admin_start_game();
      } else if (command.compare(PLAYERS) == 0) {
        return admin_players();
      } else if (command.compare(KEY) == 0) {
        return admin_key();
      } else {
        return invalid_command(command,
            {START_GAME, PLAYERS, KEY});
      }
    case PLAYING:
      if (command.compare(INFO) == 0) {
        return admin_info();
      } else if (command.compare(MAP) == 0) {
        return admin_map();
      } else if (command.compare(FORCE_FINISH) == 0) {
        return admin_force_finish();
      } else {
        return invalid_command(command, {INFO, MAP, FORCE_FINISH});
      }
    case FINISHED:
      if (command.compare(LEADERBOARD) == 0) {
        return admin_leaderboard();
      } else if (command.compare(MAP) == 0) {
        return admin_map();
      } else {
        return invalid_command(command, {LEADERBOARD, MAP});
      }
  }
}

std::string GameState::admin_start_game() {
  std::cout << "** STARTING GAME **\n";
  world_.create();
  state_ = PlayState::PLAYING;

  EventManager::end_game_event =
    event_new(EventManager::base, (evutil_socket_t) -1, 0, end_game, (void *) this);
  return START_GAME + ": SUCCESS\n";
}

std::string GameState::admin_players() {
  std::map<std::string, city_id>::iterator it;
  std::string curr_players = "key        city_name\n";
  for (it = city_map_.begin(); it != city_map_.end(); it++) {
    curr_players += it->first + " ";
    curr_players += world_.name_of(it->second) + "\n";
  }
  return curr_players;
}

std::string GameState::admin_key() {
  std::string key = generate_key();
  city_map_.insert(
      std::pair<std::string, city_id>(key, City::get_next_city_id()));
  return "NEW KEY: " + key + "\n";
}

std::string GameState::admin_info() {
  return world_.all_city_info();
}

std::string GameState::admin_map() {
  std::map<std::string, city_id>::iterator it;
  std::string map_str = "";
  for (it = city_map_.begin(); it != city_map_.end(); it++) {
    map_str += it->first + " " + world_.city_loc(it->second) + "\n";
  }
  return map_str;
}

std::string GameState::admin_force_finish() {
  std::cout << "** ADMIN FORCE GAME FINISH **\n";
  state_ = PlayState::FINISHED;
  finish_info = world_.all_city_info();
  finish_condition = "GAME OVER: ADMIN has forced finish\n";
  return FORCE_FINISH + ": SUCCESS\n";
}

std::string GameState::admin_leaderboard() {
  return finish_info;
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
    case PLAYER_JOIN:
      if (split_req.size() != 3 || command.compare(Requests::JOIN) != 0) {
        if (id != City::INVALID_CITY) {
          return "Please wait for the game to start.\n";
        } else {
          return "INVALID SYNTAX\nVALID: [player key] JOIN [city_name_no_spaces]\n";
        }
      } else {
        if (id == City::INVALID_CITY) {
          return "INVALID PLAYER KEY: [" + player_key + "]\n";
        } else {
          return player_join(id, split_req[2]);
        }
      }
    case PLAYING:
      if (id == City::INVALID_CITY) {
        return "INVALID PLAYER KEY: [" + player_key + "]\n";
      }

      if (command.compare(Requests::WORLD) * command.compare("W") == 0) {
        return player_world(id);

      } else if (command.compare(Requests::CITY) * command.compare("C") == 0) {
        return player_city(id);

      } else if (command.compare(Requests::COSTS) * command.compare("CO") == 0) {
        return player_costs(id);

      } else if (command.compare(Requests::UPGRADE) * command.compare("U") == 0) {
        return player_upgrade(id);

      } else if (command.compare(Requests::TRAIN) * command.compare("T") == 0) {
        if (split_req.size() < 3 || (!Utils::is_number(split_req[2]) &&
            split_req[2].compare("MAX") != 0)) {
          return Responses::INVALID_TRAIN;
        } else if (split_req[2].compare("MAX") == 0) {
          return player_train(id, -1);
        } else {
          int num_soldiers = Utils::safe_stoi(split_req[2]);
          return player_train(id, num_soldiers);
        }

      } else if (command.compare(Requests::ATTACK) * command.compare("A") == 0) {
        if (split_req.size() < 4 || !Utils::is_number(split_req[3])) {
          return Responses::INVALID_ATTACK;
        } else {
          int num_soldiers = Utils::safe_stoi(split_req[3]);
          return player_attack(id, split_req[2], num_soldiers);
        }
      } else {
        return "INVALID: [" + command + "]\nVALID:\n" + PLAYER_VALID_COMMANDS;
      }
    case FINISHED:
      return player_finished(id);
  }
}

std::string GameState::player_join(city_id id, std::string city_name) {
  switch (world_.add_city(id, city_name)) {
    case World::AddCityResponse::SUCCESS:
      std::cout << "** " << city_name << " has joined (id " << id << ") **" << std::endl;
      return "The city of " + city_name + " is welcomed warmly to CityBash!\n";
    case World::AddCityResponse::CITY_EXISTS:
      return "ERROR: your player key already been used.\n";
    case World::AddCityResponse::NAME_EXISTS:
      return "ERROR: " + city_name + " is already taken.\n";
  }
}

std::string GameState::player_world(city_id id) {
  return world_.other_cities_info(id);
}

std::string GameState::player_city(city_id id) {
  return world_.city_info(id);
}

std::string GameState::player_costs(city_id id) {
  return world_.city_costs(id);
}

std::string GameState::player_upgrade(city_id id) {
  return world_.city_upgrade(id);
}

std::string GameState::player_train(city_id id, int soldiers) {
  if (soldiers == 0) {
    return "TRAIN FAILURE. Cannot train 0 soldiers\n";
  }
  return world_.city_train(id, soldiers);
}

std::string GameState::player_attack(city_id from_city,
    std::string to_city_name, int soldiers) {
  if (soldiers == 0) {
    return "ATTACK FAILURE Cannot attack with 0 soldiers\n";
  }
  return world_.city_attack(from_city, to_city_name, soldiers);
}

std::string GameState::player_finished(city_id id) {
  if (City::winning_city && id == City::winning_city->get_id()) {
    return "GAME OVER: YOU have WON!\n";
  }
  return finish_condition;
}
