#! /usr/bin/python

from utils import send_command
import argparse
import time

parser = argparse.ArgumentParser()
parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('player_key')

args = parser.parse_args()

while True:
    try:
        msg = args.player_key + ' city\n'
        resp = send_command(args.host, args.port, msg)
        print resp
        if resp.startswith('INVALID') or resp.startswith('ERROR'):
            break
        time.sleep(1)
    except:
        break
