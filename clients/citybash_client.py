#!/usr/bin/python
import argparse
import utils

parser = argparse.ArgumentParser("Start a citybash client program")

parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('player_key')
parser.add_argument('city_name_no_spaces')
parser.add_argument('-r', '--rejoin', action='store_true')

args = parser.parse_args()
key = args.player_key
name = args.city_name_no_spaces

do_setup = True
if args.rejoin:
    do_setup = False

if do_setup:
    print 'Starting CityBash Game Client...'
    utils.send_hello(args.host, args.port)
    utils.send_join(args.host, args.port, key, name)
else:
    print 'Resuming CityBash Game Client for key ' + key + '...'

print '*NOTE: your player key is automatically prepended to all messages*'
utils.run(args.host, args.port, key)
