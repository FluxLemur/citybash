#include <iostream>
#include <string>

#include "game_server.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "ERROR: please start game with admin hash";
    exit(1);
  }
  std::string admin_hash = argv[1];

  GameServer gameServer(admin_hash);
  gameServer.run();
}
