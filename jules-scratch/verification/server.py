import http.server
import socketserver
import os

PORT = 8000
web_dir = os.path.join(os.path.dirname(__file__), '../../RX_FSK/data')
control_file = os.path.join(os.path.dirname(__file__), 'control.html')


class MyHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/control.html':
            self.path = control_file
            return http.server.SimpleHTTPRequestHandler.do_GET(self)
        else:
            os.chdir(web_dir)
            return http.server.SimpleHTTPRequestHandler.do_GET(self)

httpd = socketserver.TCPServer(("", PORT), MyHttpRequestHandler)

print("serving at port", PORT)
httpd.serve_forever()