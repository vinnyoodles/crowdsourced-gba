import server, emulator, sys


def usage():
    print('Invalid arguments: python %s <rom file>' % (sys.argv[0]))

if __name__ == '__main__':
    if len(sys.argv) != 2:
        usage()
        sys.exit(0)
    emulator.run(sys.argv[1])
    # server.run()
