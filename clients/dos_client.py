#! /usr/bin/python

from utils import send_command
import argparse
import time

parser = argparse.ArgumentParser()
parser.add_argument('host')
parser.add_argument('port', type=int)
parser.add_argument('-d' '--duration', type=int)

args = parser.parse_args()

duration = args.d__duration or 3
start = time.time()
count = 0

while True:
    try:
        msg = 'asdfngnqwrgwrkljadsnfiwngiwqeu023nksndjjjjjjjjjsdlfnsdjfnasdkjfnaskdjnfakjsdnfakjsdnfakjsdnfkajsdnfkasjdnfkjasndfkjansdkfjansdkjfnaskdjnfakjsdnfaskjdnf'
        resp = send_command(args.host, args.port, msg)
        if resp:
            count += 1

        if time.time() - start > duration:
            break
    except:
        break

avg = float(count) / duration
print 'Sent {} messages, with {} m/s'.format(count, avg)
