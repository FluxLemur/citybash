#! /usr/bin/python

import argparse
import os
import subprocess
import sys; sys.path.append('../clients/')

from utils import send_command

parser = argparse.ArgumentParser("Start up several citybash AIs")

parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('admin_key')
parser.add_argument('num_ai', type=int)

args = parser.parse_args()

keys = []
msg = '{} key\n'.format(args.admin_key)

for i in range(args.num_ai):
    resp = send_command(args.host, args.port, msg)
    if resp == None:
        raise IOError('Cannot connect to {} {}'.format(args.host, args.port))
    else:
        keys.append(resp.split()[-1])

try:
    os.mkdir(os.path.join(os.getcwd(), 'out'))
except:
    pass

for key in keys:
    print 'enter city name for {}'.format(key)
    city_name = raw_input()
    cmd = ['python', 'run_ai.py', args.host, str(args.port), key, city_name]
    cmd.extend(['>', 'out/ai_{}.out'.format(key)])
    subprocess.Popen(' '.join(cmd), shell=True)
