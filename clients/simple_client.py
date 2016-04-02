#!/usr/bin/python
import sys
import utils

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print "Usage: client.py [port] [host]"
        quit()

    port = int(sys.argv[1])
    host = sys.argv[2]

    print 'Starting Simple CityBash Client...'
    utils.send_hello(host, port)
    utils.run(host, port)
