#include <iostream>
#include <string>
#include <ctime>

#include "game_server.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " admin_key [seed]" << std::endl;
    exit(1);
  }

  if (argc >= 3 && Utils::is_number(argv[2])) {
    std::srand((unsigned int) std::stoi(argv[2])); // random seed
  } else {
    std::srand((unsigned int) std::time(0)); // random seed
  }

  std::string admin_key = argv[1];
  Utils::upper(admin_key);

  GameServer gameServer(admin_key);
  gameServer.run();
}
