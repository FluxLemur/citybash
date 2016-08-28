
def parse_world(string):
    '''
    Returns a list of tuples:
        (city_name, city_id, city_level, distance to city)
    '''
    try:
        city_strings = string.strip().split('\n')

        cities = []
        for city_string in city_strings[1:]:
            split = city_string.split()

            name = split[0]
            city_id = int(split[1][1:-1])
            level = int(split[2])
            distance = float(split[3])

            cities.append((name, city_id, level, distance))

        return cities

    except:
        raise


def parse_city(string):
    '''
    Returns a tuple:
        (level, gold, income, army, notifications)
    All of these fields are ints, except for notifications which is a list
    of tuples of the form:
    [time_since, event, other_city, attackers_sent, attackers_remain,
        gold_taken, defenders, defenders_remain]
    '''
    try:
        lines = string.strip().split('\n')

        level = int(lines[1].split()[1])
        gold = int(lines[2].split()[1])
        income = int(lines[3].split()[1])
        army = int(lines[4].split()[1])

        notifications = []
        for line in lines[5:]:
            split = line.split()

            time_since = int(split[1])
            if split[4] == 'attacked':
                event = 'attacked'
                other_city = split[5]
            else:
                event = 'attacked by'
                other_city = split[4]

            attackers = split[6].split('/')
            attackers_remain = int(attackers[0])
            attackers_sent = int(attackers[1])

            gold_taken = int(split[8])

            if len(split) > 10:
                defenders_ = split[10].split('/')
                defenders_remain = int(defenders_[0])
                defenders = int(defenders_[1])
            else:
                defenders_remain = -1
                defenders = -1


            notifications.append((time_since, event, other_city, attackers_sent,
                attackers_remain, gold_taken, defenders, defenders_remain))

        return (level, gold, income, army, notifications)

    except:
        raise

def parse_costs(string):
    '''
    Returns a tuple:
        (upgrade gold cost, upgrade time, train cost, train time)
    Both ints
    '''
    try:
        lines = string.strip().split('\n')

        upgrade_cost = int(lines[1].split()[1])
        upgrade_time = int(lines[1].split()[3])
        train_cost = int(lines[2].split()[1])
        train_time = int(lines[2].split()[3])

        return (upgrade_cost, upgrade_time, train_cost, train_time)

    except:
        raise
