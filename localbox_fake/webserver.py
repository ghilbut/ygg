#!/usr/bin/python
#-*- coding:utf-8 -*-
from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
import os

BASEPATH = os.path.abspath(__file__)
print BASEPATH
BASEPATH = os.path.dirname(BASEPATH)
print BASEPATH

HTDOCS = os.path.join(BASEPATH, 'htdocs')
print HTDOCS

CONTENT_TYPE = {'.html': 'text/html'
              , '.css' : 'text/css'
              , '.js'  : 'application/javascript'
              , '.gif' : 'image/gif'
              , '.jpg' : 'image/jpg'
              , '.png' : 'image/png'}

PORT_NUMBER = 81

#This class will handles any incoming request from
#the browser 
class myHandler(BaseHTTPRequestHandler):

  def do_GET(self):
    if self.path == '/':
      path = '/index.html'
    else:
      path = self.path

    try:
      idx = path.rfind('.')
      ext = path[idx:].lower()

      if ext in CONTENT_TYPE:
        content_type = CONTENT_TYPE[ext]
      else:
        content_type = 'application/octet-stream'

      filepath = path.replace('/', os.sep)
      filepath = os.path.join(HTDOCS, '.' + filepath)
      filepath = os.path.abspath(filepath)

      f = open(filepath)
      self.send_response(200)
      self.send_header('Content-type', content_type)
      self.end_headers()
      self.wfile.write(f.read())
      f.close()

    except IOError:
      self.send_error(404, 'File Not Found: %s' % self.path)


try:
  #Create a web server and define the handler to manage the
  #incoming request
  server = HTTPServer(('', PORT_NUMBER), myHandler)
  print 'Started httpserver on port ' , PORT_NUMBER
  
  #Wait forever for incoming htto requests
  server.serve_forever()

except KeyboardInterrupt:
  print '^C received, shutting down the web server'
  server.socket.close()