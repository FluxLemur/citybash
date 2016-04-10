#! /usr/bin/python2.7
import socket
import sys
import time
from simple_ai import SimpleAI

def send_command(host, port, message):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((host, port))
        s.send(message)
        return s.recv(2000)
    except socket.error:
        print 'ERROR: Cannot connect to {}:{}'.format(host, port)
        quit()

if len(sys.argv) < 5:
    print "[host] [port] [key] [city_name_no_spaces]"
    quit()

host = sys.argv[1]
port = int(sys.argv[2])
key = sys.argv[3]
name = sys.argv[4]

print 'Running SimpleAI...'
print send_command(host, port, '%s JOIN %s\n' %(key, name))

ai = SimpleAI()

while True:
    time.sleep(0.2)
    resp = send_command(host, port, key + ' city\n')
    print resp
    if 'ERROR' not in resp and 'INVALID' not in resp:
        break

# assume the game has now started
while True:
    time.sleep(0.5)
    resp = send_command(host, port, key + ' ' + ai.next_command() + '\n')
    print resp
    ai.add_response(resp)
