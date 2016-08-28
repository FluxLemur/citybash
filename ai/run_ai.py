#! /usr/bin/python2.7
import sys; sys.path.append('../clients/utils.py')
import time
from simple_ai import SimpleAI
from utils import send_command

if len(sys.argv) < 5:
    print "[host] [port] [key] [city_name_no_spaces]"
    quit()

host = sys.argv[1]
port = int(sys.argv[2])
key = sys.argv[3]
name = sys.argv[4]

print 'Running SimpleAI...'
print send_command(host, port, '%s JOIN %s\n' %(key, name))

ai = SimpleAI()

while True:
    time.sleep(0.5)
    resp = send_command(host, port, key + ' city\n')
    print resp
    if 'ERROR' not in resp and 'INVALID' not in resp:
        break

# assume the game has now started
while True:
    time.sleep(0.5)
    resp = send_command(host, port, key + ' ' + ai.next_command() + '\n')
    print resp
    ai.add_response(resp)
