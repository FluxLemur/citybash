import sys; sys.path.append('../clients/utils.py')
import utils

class LoginInfo:
    def __init__(self, host, port, key):
        self.host = host
        self.port = port
        self.key = key

class Messenger:
    def __init__(self, login_info):
        self.login_info = login_info

    def send(self, msg):
        return utils.send_command(self.host, self.port, msg)

    def world(self):
        return Message(self.login_info, "w")

    def city(self):
        return Message(self.login_info, "c")

    def upgrade(self):
        return Message(self.login_info, "u")

    def train(self, number):
        return Message(self.login_info, "t {0}".format(number))

    def max_train(self):
        return Message(self.login_info, "t max")

    def attack(self, city, number):
        return Message(self.login_info, "a {0} {1}".format(city, number))

class Message:
    def __init__(self, login_info, msg):
        self.host = login_info.host
        self.port = login_info.port
        self.key = login_info.key
        self.msg = msg

    def get_msg(self):
        return "{0} {1}\n".format(self.key, self.msg)

    def send(self):
        msg = self.get_msg()
        return utils.send_command(self.host, self.port, msg)

    def __eq__(self, other):
        return self.get_msg() == other.get_msg()
