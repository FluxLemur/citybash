import numpy.random as RA
class Battle:
    def __init__(self, a, d, gold):
        ''' the attackers [a] fight the defenders [d], and take some amount of gold
            with them if they win '''
        self.winner = 'defenders'
        self.attackers = a
        self.defenders = d
        self.gold = gold

        if a > d:
            self.winner = 'attackers'

        self.attackers_remaining = max(0, a - d)
        self.defenders_remaining = max(0, d - a)
        self.gold_stolen = 0

        if self.winner == 'attackers':
            self.gold_stolen = a

    def __str__(self):
        fstr = '{} attackers, {} defenders, {} gold.\n'
        fstr += '  {:2} remaining attackers\n'
        fstr += '  {:2} remaining defenders\n'
        fstr += '  {:2} gold stolen\n'
        fstr += '     {} won\n'

        return fstr.format(self.attackers, self.defenders, self.gold,
                           self.attackers_remaining,
                           self.defenders_remaining,
                           self.gold_stolen,
                           self.winner)


battles = [Battle(*(RA.randint(20), RA.randint(20), RA.randint(100))) for i in range(10)]
for battle in battles:
    print str(battle)
