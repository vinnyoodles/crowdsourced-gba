import os.path, tornado
from tornado import websocket, web, ioloop

class Server:
    # Store all active clients in a set.
    clients = set()
    # Data to send to the client when entering.
    metadata = dict()
    # The emulator instance
    core = None

    #Stores all commands that clients used in the game already
    allLogging = list()
    # Stores the current command used
    newCmd = dict()

    # mapping of keynames that the client will use
    keymap = {0: "a", 1: "b", 2: "select", 3: "Start", 4: "right",
              5: "left", 6: "up", 7: "down", 8: "r", 9: "l"}

    def __init__(self):
        self._settings = dict(
            template_path=os.path.join(os.path.dirname(__file__), 'static'),
            static_path=os.path.join(os.path.dirname(__file__), 'static'),
            debug=True,
            autoreload=True
        )
        self._handlers = [
            (r'/', Server.IndexHandler),
            (r'/ws', Server.SocketHandler),
            (r'/favicon.ico', web.StaticFileHandler, { 'path': '.' })
        ]

        self.app = web.Application(self._handlers, **self._settings)

    class IndexHandler(web.RequestHandler):
        def get(self):
            self.render('index.html')

    class SocketHandler(websocket.WebSocketHandler):
        def open(self):
            Server.clients.add(self)
            self.write_message(Server.metadata)
            if len(Server.allLogging) > 0:
                self.write_message(Server.allLogging)

        def on_message(self, key):
            if Server.core is not None:
                cmd = dict()
                temp[str(self)] = Server.keymap.get(int(key))
                Server.newCmd['clientCmd'] = cmd
                Server.allLogging.append(cmd)
                Server.core.push_key(int(key))

        def on_close(self):
            Server.clients.remove(self)

        def check_origin(self, orgin):
            return True

    def set_core(self, _core):
        Server.core = _core
        Server.metadata['width'] = _core.width
        Server.metadata['height'] = _core.height

    def emit_frame(self, data):
        if data is not None and len(data) > 0:
            @tornado.gen.coroutine
            def stream_frame(self):
                for client in Server.clients:
                    yield client.write_message(data, binary=True)
                    #Send the recent command to the user
                    if Server.newCmd is not None:
                        yield client.write_message(Server.newCmd)

            tornado.ioloop.IOLoop.current().spawn_callback(stream_frame, self)

    def listen(self, port):
        self.app.listen(port)


if __name__ == '__main__':
    Server().listen(8888)
    ioloop.IOLoop.instance().start()
