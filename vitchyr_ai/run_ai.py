#! /usr/bin/python2.7
import socket
import sys
import time
from vitchyr_ai import VitchyrAI
from clients.utils import send_command
from messenger import LoginInfo

if len(sys.argv) < 5:
    print "[host] [port] [key] [city_name_no_spaces]"
    quit()

host = sys.argv[1]
port = int(sys.argv[2])
key = sys.argv[3]
name = sys.argv[4]

print 'Running VitchyrAI...'
print send_command(host, port, '%s JOIN %s\n' %(key, name))

login_info = LoginInfo(host, port, key)
TRAIN_FREQ = 2 # seconds
ai = VitchyrAI(login_info, TRAIN_FREQ)

while True:
    time.sleep(0.5)
    resp = send_command(host, port, key + ' city\n')
    print resp
    if 'ERROR' not in resp and 'INVALID' not in resp:
        break

# assume the game has now started
while True:
    time.sleep(0.5)
    ai.step()
