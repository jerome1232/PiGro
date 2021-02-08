import socket
import logging

class Server:
    """
    A simple socket server to listen for controls
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

        try:
            os.unlink(self.location)
        except OSError:
            if os.pathexists(server_address):
                raise


    def get_data(self):
        """Retrieves data from a socket connection"""
        data_recvd = b''

        with self.sock as s:
            s.bind(self.location)
            s.listen()
            conn, addr = s.accept()
            with conn:
                logging.info('Connection from:' + addr)
                while True:
                    data = conn.recv(1024)
                    if not data:
                        return data_recvd.decode('UTF-8')
                    data_recvd += data
