#ifndef BATTLE_H
#define BATTLE_H

class Battle {
  private:
    int attackers_remaining_;
    int defenders_remaining_;

  public:
    Battle(int num_attackers, int num_defenders, int defender_level);

    int attackers_remaining();
    int defenders_remaining();
};
#endif // BATTLE_H
