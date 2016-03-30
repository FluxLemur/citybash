#!/usr/bin/python
import sys
import utils

if __name__ == '__main__':
    if len(sys.argv) != 5:
        print "Usage: {} [host] [port]".format(sys.argv[0])\
              + " [player key] [city_name_no_spaces]"
        quit()

    host = sys.argv[1]
    port = int(sys.argv[2])
    key = sys.argv[3]
    name = sys.argv[4]

    print 'Starting CityBash Game Client...'
    utils.send_hello(host, port)
    utils.send_join(host, port, key, name)

    print '*NOTE: your player key is automatically prepended to all messages*'
    utils.run(host, port, key)
