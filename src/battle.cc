#include "battle.h"
#include "city.h"

Battle::Battle(int num_attackers, int num_defenders, int defender_level) {
  float multiplier = (float) City::defense_multiplier[defender_level - 1];
  float defender_power = num_defenders * multiplier + 1;
  float total_power = num_attackers + defender_power;


  attackers_remaining_ = int(num_attackers * (num_attackers / total_power));
  defenders_remaining_ = int(num_defenders * (defender_power / total_power));
}

int Battle::attackers_remaining() {
  return attackers_remaining_;
}

int Battle::defenders_remaining() {
  return defenders_remaining_;
}
