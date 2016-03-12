#include <iostream>
#include <string>
#include <ctime>

#include "game_server.h"
#include "utils.h"

int main(int argc, char** argv) {
  std::srand((unsigned int) std::time(0)); // random seed

  if (argc < 2) {
    std::cout << "ERROR: please start game with admin key" << std::endl;
    exit(1);
  }
  std::string admin_key = argv[1];
  Utils::upper(admin_key);

  GameServer gameServer(admin_key);
  gameServer.run();
}
