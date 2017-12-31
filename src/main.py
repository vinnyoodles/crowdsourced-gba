import server, emulator, sys, threading
import tornado.ioloop

def usage():
    print('Invalid arguments: python %s <rom file>' % (sys.argv[0]))

def thread_function(core):
    core.run()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        usage()
        sys.exit(0)

    web_server = server.Server()
    web_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()

    core = emulator.Emulator(sys.argv[1], web_server)
    web_server.set_core(core)
    emulator_thread = threading.Thread(target=thread_function, args=(core,))
