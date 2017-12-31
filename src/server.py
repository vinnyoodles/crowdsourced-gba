import os.path
from tornado import websocket, web, ioloop

# Store all active clients in a set.
clients = set()
metadata = dict()

class IndexHandler(web.RequestHandler):
    def get(self):
        self.render('index.html')

class SocketHandler(websocket.WebSocketHandler):
    def open(self):
        clients.add(self)
        self.write_message(metadata)

    def on_message(self, message):
        self.write_message(message[::])

    def on_close(self):
        clients.remove(self)

    def check_origin(self, orgin):
        return True

class Server(web.Application):
    def __init__(self):
        self._settings = dict(
            template_path=os.path.join(os.path.dirname(__file__), 'static'),
            static_path=os.path.join(os.path.dirname(__file__), 'static'),
            debug=True,
            autoreload=True
        )
        self._handlers = [
            (r'/', IndexHandler),
            (r'/ws', SocketHandler),
            (r'/favicon.ico', web.StaticFileHandler, { 'path': '.' })
        ]

        web.Application.__init__(self, self._handlers, **self._settings)

    def set_core(self, core):
        self.core = core
        metadata['width'] = core.width
        metadata['height'] = core.height

    def emit_frame(self, data):
        if len(data) <= 0:
            return
        for client in clients:
            client.write_message(data, binary=True)


if __name__ == '__main__':
    Server().listen(8888)
    ioloop.IOLoop.instance().start()
