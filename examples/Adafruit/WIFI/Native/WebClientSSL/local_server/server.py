import BaseHTTPServer, SimpleHTTPServer
import ssl
import sys

PORT = 443

if (len(sys.argv) > 1):
  PORT = int(sys.argv[1])

Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
httpd = BaseHTTPServer.HTTPServer(("", PORT), Handler)
httpd.socket = ssl.wrap_socket (httpd.socket, keyfile='server.key', certfile='server.crt', server_side=True)
print "serving at port", PORT
httpd.serve_forever()
