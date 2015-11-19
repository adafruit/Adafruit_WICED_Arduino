import SimpleHTTPServer
import SocketServer
import sys

PORT = 80

if (len(sys.argv) > 1):
  PORT = int(sys.argv[1])

Handler = SimpleHTTPServer.SimpleHTTPRequestHandler

httpd = SocketServer.TCPServer(("", PORT), Handler)

print "serving at port", PORT
httpd.serve_forever()
