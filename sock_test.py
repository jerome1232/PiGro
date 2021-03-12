import logging
import Sock_Serv
import time

def main():

   logging.basicConfig(level=logging.DEBUG)

   server = Sock_Serv.Server(location='test.sock')

   logging.debug('Socket created')

   while True:
      logging.debug('reading socket')
      dat = server.get_data()
      if dat is not None:
         logging.debug('data: ' + dat)
      server.setup()

      logging.debug('Sleeping for 5 seconds')
      time.sleep(5)
      logging.debug('Done sleeping')


if __name__ == "__main__":
   main()