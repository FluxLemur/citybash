import numpy.random as RA

defense_multiplier = [1.2, 1.3, 1.4, 1.5]
cache_size = [5, 15, 45, 135]

class Battle:
    def __init__(self, attackers, defenders, defender_city_level, gold):
        assert defender_city_level > 0 and defender_city_level < 5
        assert gold > 0
        assert attackers > 0
        assert defenders > 0

        attackers_win = False
        multiplier = defense_multiplier[defender_city_level - 1]
        defender_power = int(defenders * multiplier)
        gold_stolen = 0

        if attackers > defender_power:
            attackers_win = True
            attackers_remaining = attackers - defender_power
            defenders_remaining = 0
        else:
            attackers_remaining = 0
            defenders_remaining = defenders - int(attackers / multiplier)

        gold_avail = max(0, gold - cache_size[defender_city_level - 1])
        gold_stolen = min(gold_avail, attackers_remaining * 2)

        res = '{} attackers, {} defenders, {} defender city level, {} gold.\n'\
                .format(attackers, defenders, defender_city_level, gold)

        res += '  ATTACK '
        if attackers_win:
            res += 'WIN '
        else:
            res += 'LOSE '

        res += '{}/{} return with {} gold'\
                .format(attackers_remaining, attackers, gold_stolen)

        if attackers_win:
            res += ', [defending-city-name] remaining: '
            res += '{}/{}'.format(defenders_remaining, defenders)

        self._result = res + '\n'

    def __str__(self):
        return self._result

print '''Below are some annotated examples of battles. The line that begins with
ATTACK is the response that the game server would return to the player issuing
the attack.\n'''

print '** In the case of ties, defenders win   '
print "   Note: the response doesn't disclose information about the defenders **"
print Battle(attackers=1, defenders=1, defender_city_level=1, gold=10)

print '** The 2 remaining soldiers carry 2 gold each'
print '   Note: the attacker now knows there are no more soldiers in the city **'
print Battle(3, 1, 1, 10)

print '** The remaining 3 soldiers can carry 6 gold, but the city cache stores'
print '   5 of the total 10, so only 5 can be stolen **'
print Battle(4, 1, 1, 10)

print '** At higher city levels, the defense multiplier can be powerful **'
print Battle(15, 10, 4, 100)

print '** At high city levels, the cache size is substantial **'
print Battle(100, 10, 4, 130)
