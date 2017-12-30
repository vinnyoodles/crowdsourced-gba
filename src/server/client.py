import os.path
from tornado import websocket, web, ioloop

class IndexHandler(web.RequestHandler):
    def get(self):
        self.write('Hello World!')

def run():
    print('starting client')
    client = web.Application([
        (r'/', IndexHandler), 
        (r'/favicon.ico', web.StaticFileHandler, {'path': 'static'})
    ])
    client.listen(3000)
    ioloop.IOLoop.instance().start()