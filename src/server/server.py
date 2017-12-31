import os.path
from tornado import websocket, web, ioloop

class IndexHandler(web.RequestHandler):
    def get(self):
        self.render("index.html")

class SocketHandler(websocket.WebSocketHandler):
    def open(self):
        pass

    def on_message(self, message):
        self.write_message(message[::])

    def on_close(self):
        pass

    def check_origin(self, orgin):
        return True

app = web.Application([
    (r'/', IndexHandler),
    (r'/ws', SocketHandler),
    (r'/(favicon.ico)', web.StaticFileHandler, {'path': '../'}),
])

if __name__ == '__main__':
    app.listen(8888)
    ioloop.IOLoop.instance().start()
