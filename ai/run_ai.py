#! /usr/bin/python2.7
import argparse
import sys; sys.path.append('../clients/')
import time

from simple_ai import SimpleAI
from utils import send_command

parser = argparse.ArgumentParser()
parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('player_key')
parser.add_argument('city_name_no_spaces')

args = parser.parse_args()
key = args.player_key
name = args.city_name_no_spaces

print 'Running SimpleAI...'
print send_command(args.host, args.port, '{} JOIN {}\n'.format(key, name))

# wait for the game to start
while True:
    time.sleep(0.5)
    resp = send_command(args.host, args.port, key + ' city\n')
    print resp
    if 'ERROR' not in resp and 'INVALID' not in resp:
        break


ai = SimpleAI()
while True:
    time.sleep(0.5)
    msg = key + ' ' + ai.get_command() + '\n'
    resp = send_command(args.host, args.port, msg)
    print resp
    ai.add_response(resp)
