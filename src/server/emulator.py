import socket

EMULATOR_IP = 'localhost'
EMULATOR_PORT = 2578
BUFFER_SIZE = 1024

def run():
    print('connecting to emulator')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((EMULATOR_IP, EMULATOR_PORT))
    while True:
        chunk = s.recv(BUFFER_SIZE)
        if chunk == b'':
            return           
        print(chunk.decode('utf-8'))