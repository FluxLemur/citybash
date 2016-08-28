import random
import ai_utils

class SimpleAI:
    def __init__(self):
        self.commands = []
        self.responses = []
        self.turn = -1
        self.other_cities = []
        self.level = 0
        self.gold = 0
        self.income = 0
        self.army = 0

        self.upgrade_cost = -1
        self.upgrade_time = -1
        self.train_cost = -1
        self.train_time = -1

        self.notifications = []
        self.get_costs = False

    def get_command(self):
        command = self.next_command()
        self.commands.append(command)
        return command

    def next_command(self):
        self.turn += 1

        if self.turn == 0:
            return 'world'

        elif self.get_costs:
            self.get_costs = False
            return 'costs'

        elif self.turn % 3 == 1:
            return 'city'

        elif self.turn % 3 == random.randint(0, 2) and self.army > 0 and \
                len(self.other_cities) > 0:
            city = random.choice(self.other_cities)[1]
            n = random.randint(1, self.army)
            return 'attack {} {}'.format(city, n)

        elif self.gold > self.train_cost and \
                self.turn % 2 == random.randint(0, 2):
            return 'train max'

        else:
            return 'city'


    def add_response(self, resp):
        if resp.startswith('WORLD'):
            self.other_cities = ai_utils.parse_world(resp)

        elif resp.startswith('CITY'):
            new_level, self.gold, self.income, self.army, new_notifications = \
                ai_utils.parse_city(resp)

            # TODO: there's going to be repeat here
            self.notifications.append(new_notifications)

            # used to update costs next command
            if new_level == self.level + 1:
                self.get_costs = True

            self.level = new_level

        elif resp.startswith('COSTS'):
            self.upgrade_cost, self.upgrade_time, self.train_cost, \
                self.train_time = ai_utils.parse_costs(resp)

        self.responses.append(resp)
