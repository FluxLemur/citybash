import socket

def send(socket, message):
    # In Python 3, must convert message to bytes explicitly.
    # In Python 2, this does not affect the message.
    socket.sendall(message.encode('utf-8'))

def send_command(host, port, message):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((host, port))
        send(s, message)
        return s.recv(2000)
    except socket.error:
        print 'ERROR: Cannot connect to {}:{}'.format(host, port)
        return None
    except:
        raise

def run(host, port, key=None):
    while True:
        try:
            msg = raw_input() +'\n'
        except KeyboardInterrupt:
            quit()

        if msg.upper() == 'Q' or msg.upper() == 'QUIT':
            quit()

        if key is not None:
            msg = key + ' ' + msg

        print send_command(host, port, msg)

def send_hello(host, port):
    resp = send_command(host, port, 'CLIENT_HELLO %s\n' % socket.gethostname())
    if resp is None:
        exit(1)
    else:
        print resp

def send_join(host, port, key, name):
    resp = send_command(host, port, '%s JOIN %s\n' %(key, name))
    if resp.startswith('ERROR') or resp.startswith('INVALID'):
        print resp
        quit()
    print resp
