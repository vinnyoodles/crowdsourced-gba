import os.path
from tornado import websocket, web, ioloop
import socket

class Indexhandler(web.RequestHandler):
    def get(self):
        self.render("index.html")

class SocketHandler(websocket.WebSocketHandler):
    def open(self):
        print 'new connection'

    def on_message(self, message):
        print 'message received: %s' % message

        print 'send the message back: %s' % message[::]
        self.write_message(message[::])

    def on_close(self):
        print 'connection closed'

    def check_origin(self, orgin):
        return True

app = web.Application([
    (r'/', Indexhandler),
    (r'/ws', SocketHandler),
    (r'/(favicon.ico)', web.StaticFileHandler, {'path': '../'}),
])

if __name__ == '__main__':
    app.listen(8888)
    myIP = socket.gethostbyname(socket.gethostname())
    print '*** Websocket Server Started at %s***' % myIP
    ioloop.IOLoop.instance().start()
