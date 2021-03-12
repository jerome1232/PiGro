import logging
import Sock_Client

def main():

   client = Sock_Client.Sock_Client(location='test.sock')

   logging.debug('Socket create')

   dat = 'This is a test'

   client.write(dat)

if __name__ == "__main__":
   main()