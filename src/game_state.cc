#include <iostream>
#include <string>

#include "constants.h"
#include "game_state.h"
#include "utils.h"

GameState::GameState() {
  state_ = PlayState::ADMIN_WAIT;
}

/**
 * Example: invalid_command("yo", {"no", "help"}) returns:
 * [yo] INVALID
 * VALID [no] [help]
 */
std::string GameState::invalid_command(std::string command,
    std::vector<std::string> valid_commands) {
  std::string response = "[" + command + "] INVALID\n";
  response += "VALID";
  for (auto const &valid : valid_commands) {
    response += " [" + valid + "]";
  }
  response += "\n";

  return response;
}


/******************************************************************************/
// Admin Requests
/******************************************************************************/
std::string GameState::PLAYER_JOIN_REQ = "PLAYER_JOIN";
std::string GameState::SERVER_INFO = "SERVER_INFO";
std::string GameState::START_GAME = "START_GAME";
std::string GameState::PLAYERS = "PLAYERS";
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
      } else {
        return invalid_command(command,
            {START_GAME, PLAYERS, SERVER_INFO});
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
  state_ = PlayState::PLAYER_JOIN;
  return PLAYER_JOIN_REQ + ": SUCCESS\n";
}

std::string GameState::admin_server_info() {
  return SERVER_INFO + ": TODO\n";
}

std::string GameState::admin_start_game() {
  state_ = PlayState::PLAYING;
  return START_GAME + ": SUCCESS\n";
}

std::string GameState::admin_players() {
  return PLAYERS + ": TODO\n";
}

std::string GameState::admin_stats() {
  return STATS + ": TODO\n";
}

std::string GameState::admin_map() {
  return MAP + ": TODO\n";
}

std::string GameState::admin_force_finish() {
  state_ = PlayState::FINISHED;
  return FORCE_FINISH + "SUCCESS\n";
}

std::string GameState::admin_leaderboard() {
  return LEADERBOARD + ": TODO\n";
}

std::string GameState::admin_terminate() {
  return TERMINATE + ": CityBash terminating...\n";
}


/******************************************************************************/
// Player Requests
/******************************************************************************/

std::string GameState::player_request(std::string command, city_id id,
    std::vector<std::string> args) {

  if (command.compare(Requests::WORLD) == 0) {
    return get_world_info(id);

  } else if (command.compare(Requests::CITY) == 0) {
    return get_city_info(id);

  } else if (command.compare(Requests::COSTS) == 0) {
    return get_costs_info(id);

  } else if (command.compare(Requests::UPGRADE) == 0) {
    return upgrade_city(id);

  } else if (command.compare(Requests::TRAIN) == 0) {
    if (args.size() == 0 || !Utils::is_number(args[0])) {
      return Responses::INVALID_TRAIN;
    } else {
      int num_soldiers = std::stoi(args[0], nullptr, 10);
      return train_soldiers(id, num_soldiers);
    }

  } else if (command.compare(Requests::ATTACK) == 0) {
    if (args.size() < 2 || !Utils::is_number(args[1])) {
      return Responses::INVALID_ATTACK;
    } else {
      int num_soldiers = std::stoi(args[1], nullptr, 10);
      return start_attack(id, args[0], num_soldiers);
    }
  }

  return Responses::INVALID;
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
