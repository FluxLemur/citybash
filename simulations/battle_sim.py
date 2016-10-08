defense_multiplier = [1.1, 1.15, 1.2, 1.25]
cache_size = [5, 15, 45, 135]

class Battle:
    '''
    Represents a battle in which city A attacks city B
    '''

    def __init__(self, attackers, defenders, level, gold):
        '''
        Defender sees:
        [city name] attacked [#]/[#] took [#] gold, [#]/[#] defenders remained

        Attacker sees:
        attacked [city name] [#]/[#] took [#] gold{, [#]/[#] defenders remained}
        '''

        assert level in (1,2,3,4)
        assert gold >= 0
        assert attackers > 0
        assert defenders >= 0

        attackers_win = False
        gold_stolen = 0

        defenders_effective = defenders + 1
        multiplier = defense_multiplier[level - 1]
        defender_power = int(defenders_effective * multiplier)
        attacker_power = attackers

        self._result = '{} attackers, {} defenders, {} city levels, {} gold\n'
        self._result = self._result.format(attackers, defenders, level, gold)

        total_power = float(attacker_power + defender_power)

        attackers_remaining = int(attackers *
                                  (attacker_power / total_power))
        defenders_remaining = int(defenders *
                                  (defender_power / total_power))

        gold_avail = max(0, gold - cache_size[level - 1])
        gold_stolen = min(gold_avail, attackers_remaining * 3)

        self._result += 'A attacked {}/{} took {} gold, '.format(
                        attackers_remaining, attackers,
                        gold_stolen)
        self._result += '{}/{} defenders remained'.format(
                         defenders_remaining, defenders)
        self._result += '\n'

        self._result += 'attacked B {}/{} took {} gold'.format(
                        attackers_remaining, attackers,
                        gold_stolen)

        if attackers_remaining > 0:
            self._result += ', {}/{} defenders remained'.format(
                            defenders_remaining, defenders)

    def __str__(self):
        return self._result

print '''Below are some annotated examples of battles. They are from the
perspective of a city named A attacking a city named B.

=== comments enclosed in equals ===
1st line: the battle conditions
2st line: what defender (B) sees
3nd line: what attacker (A) sees\n'''

examples = []
delim = '='*80


def print_examples(examples):
    for note, battle in examples:
        print '\n'.join([delim, note, delim, str(battle), ''])

examples.append((
'''One attacker remains and takes back 3 gold.''',
    Battle(attackers=2, defenders=0, level=1, gold=10)
))

examples.append((
'''Because the attacker lost all his soldiers, he doesn't get any information
about the number defenders.''',
    Battle(attackers=2, defenders=5, level=1, gold=10)
))

examples.append((
'''In the case of ties, defenders have a slight advantage due to the defense
multiplier.''',
    Battle(attackers=10, defenders=10, level=1, gold=30)
))

examples.append((
'''The remaining 2 soldiers could carry 6 gold, but the city cache stores
5 of the total 10, so only 5 can be stolen.''',
    Battle(4, 1, 1, 10)
))

examples.append((
'''At high city levels, the cache size is substantial and the defense
multiplier is greater.''',
    Battle(50, 50, 4, 150)
))

print_examples(examples)
