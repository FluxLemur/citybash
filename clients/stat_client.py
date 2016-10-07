#! /usr/bin/python

import argparse
import os
import time

from utils import send_command

parser = argparse.ArgumentParser()
parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('player_key')

args = parser.parse_args()

while True:
    time.sleep(1)
    city_msg = args.player_key + ' city\n'
    city_resp = send_command(args.host, args.port, city_msg)

    world_msg = args.player_key + ' world\n'
    world_resp = send_command(args.host, args.port, world_msg)

    os.system('clear')
    print str(city_resp)
    print str(world_resp)

    if city_resp is None or city_resp.startswith('INVALID') \
                         or city_resp.startswith('ERROR'):
        break
