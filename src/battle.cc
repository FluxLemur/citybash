#include "battle.h"
#include "city.h"

Battle::Battle(int num_attackers, int num_defenders, int defender_level) {
  double multiplier = City::defense_multiplier[defender_level - 1];
  int defender_power = int(num_defenders * multiplier);

  if (num_attackers > defender_power) {
    attackers_win_ = true;
    attackers_remaining_ = num_attackers - defender_power;
    defenders_remaining_ = 0;
  } else {
    attackers_win_ = false;
    attackers_remaining_ = 0;
    defenders_remaining_ = num_defenders - int(double(num_attackers) / multiplier);
  }
}

bool Battle::attackers_win() {
  return attackers_win_;
}

int Battle::attackers_remaining() {
  return attackers_remaining_;
}

int Battle::defenders_remaining() {
  return defenders_remaining_;
}
