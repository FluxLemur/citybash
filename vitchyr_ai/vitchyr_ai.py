from messenger import Messenger

class VitchyrAI:
    def __init__(self, login_info, train_freq):
        self.commands = []
        self.responses = []
        self.messenger = Messenger(login_info)

        self.freq = int(train_freq / 0.5)
        print(self.freq)
        self.i = 0

    def get_next_command(self):
        if self.i % self.freq == 0:
            command = self.messenger.train(1)
        else:
            command = self.messenger.upgrade()

        self.commands.append(command)
        self.i = self.i + 1
        return command

    def add_response(self, resp):
        self.responses.append(resp)

    def step(self):
        next_command = self.get_next_command()
        resp = next_command.send()
        self.add_response(resp)

        print(resp)
