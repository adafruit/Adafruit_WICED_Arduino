import BaseHTTPServer, SimpleHTTPServer
import ssl
import sys

PORT = 443

if (len(sys.argv) > 1):
  PORT = int(sys.argv[1])

httpd = BaseHTTPServer.HTTPServer(("", PORT), SimpleHTTPServer.SimpleHTTPRequestHandler)
httpd.socket = ssl.wrap_socket (httpd.socket, certfile='server.pem', server_side=True)
print "serving at port", PORT
httpd.serve_forever()
