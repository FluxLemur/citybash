#!/usr/bin/python
import sys
import utils

if __name__ == '__main__':
    if len(sys.argv) < 5:
        print "Usage: {} [host] [port]".format(sys.argv[0])\
              + " [player key] [city_name_no_spaces]"
        quit()

    do_setup = True
    if len(sys.argv) == 6 and sys.argv[5] == 'rejoin':
        do_setup = False

    host = sys.argv[1]
    port = int(sys.argv[2])
    key = sys.argv[3]
    name = sys.argv[4]

    if do_setup:
        print 'Starting CityBash Game Client...'
        utils.send_hello(host, port)
        utils.send_join(host, port, key, name)
    else:
        print 'Resuming CityBash Game Client for key ' + key + '...'

    print '*NOTE: your player key is automatically prepended to all messages*'
    utils.run(host, port, key, shortstat=True)
