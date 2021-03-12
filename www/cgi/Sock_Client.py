import socket
import logging

class Sock_Client:
   """
   A simple socket client to read data for a Unix Domain Socket.
   """

   def __init__(self, sock=None, location=None):
      if location is None:
         self.location = 'www/sock/data.sock'
      else:
         self.location = location

      if sock is None:
         self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
      else:
         self.sock = sock

   def write(self, data=None):
      ''' Sends data to socket'''
      if data is None:
         logging.info('No data to write!')
         return

      try:
         self.sock.connect(self.location)
      except socket.error:
         logging.error("Couldn't connect to" + self.location)

      try:
         self.sock.sendall(data.encode())
      finally:
         self.sock.shutdown(socket.SHUT_RDWR)
         self.sock.close()
         logging.info('done')
