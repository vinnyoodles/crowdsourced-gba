import os.path
from tornado import websocket, web, ioloop

class IndexHandler(web.RequestHandler):
    def get(self):
        self.write('Hello World!')

server = web.Application(
    [(r'/', IndexHandler), (r'/favicon.ico', web.StaticFileHandler, {'path': 'static'})]
)

if __name__ == '__main__':
    server.listen(3000)
    ioloop.IOLoop.instance().start()

