class SimpleAI:
    def __init__(self):
        self.commands = []
        self.responses = []

    def next_command(self):
        command = 'train 1'

        self.commands.append(command)
        return command

    def add_response(self, resp):
        self.responses.append(resp)
