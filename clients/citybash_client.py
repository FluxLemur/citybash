#!/usr/bin/python
import sys
import socket
import datetime

def send(socket, message):
    # In Python 3, must convert message to bytes explicitly.
    # In Python 2, this does not affect the message.
    socket.send(message.encode('utf-8'))

def send_command(host, port, message):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))

    send(s, message)
    print(s.recv(300))

def run(host, port):
    player_key = ''

    while True:
        msg = raw_input() +'\n'
        if msg.upper() == 'Q' or msg.upper() == 'QUIT':
            return

        try:
            send_command(host, port, msg)
        except socket.error:
            print 'Game server is down'
            quit()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print "Usage: client.py [server host] [server port]"
        quit()

    host = sys.argv[1]
    port = int(sys.argv[2])

    print 'Starting CityBash Game Client...'
    try:
        send_command(host, port, 'CLIENT_HELLO %s\n' % socket.gethostname())
    except socket.error:
        print 'Cannot connect to {}:{}'.format(host, port)
        quit()
    run(host, port)
