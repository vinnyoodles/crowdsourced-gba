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
    all_logs = dict()
    all_logs['event'] = 'all actions'
    cmd_list = []
    # Stores the current command used
    last_cmd = dict()
    last_cmd['event'] = 'new action'

    # mapping of keynames that the client will use
    KEYMAP = {0: "a", 1: "b", 2: "select", 3: "Start", 4: "right",
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
            self.write_message(Server.all_logs)


        def on_message(self, key):
            if Server.core is not None:
                cmd = str(self) + ': ' + str(Server.KEYMAP.get(int(key)))
                Server.last_cmd['data'] = cmd
                Server.cmd_list.append(cmd)
                Server.all_logs['data'] = Server.cmd_list
                Server.core.push_key(int(key))
                #Send the recent command to the user
                for client in Server.clients:
                    if Server.last_cmd is not None:
                        client.write_message(Server.last_cmd)

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

            tornado.ioloop.IOLoop.current().spawn_callback(stream_frame, self)

    def listen(self, port):
        self.app.listen(port)


if __name__ == '__main__':
    Server().listen(8888)
    ioloop.IOLoop.instance().start()
