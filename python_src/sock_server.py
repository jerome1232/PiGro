import logging
import os
import socket


class SockServer:
    """
    A simple socket server to listen for controls
    """

    def __init__(self, location=None):
        if location is None:
            self.location = '../www/sock/data.sock'
        else:
            self.location = location

        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        self.setup()

    def setup(self):
        try:
            os.unlink(self.location)
        except OSError:
            if os.path.exists(self.location):
                raise

        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.settimeout(1)
        self.sock.bind(self.location)
        self.sock.listen()

    def get_data(self):
        """Retrieves data from a socket connection"""
        data_recvd = b''

        with self.sock as s:
            try:
                conn, addr = s.accept()
                with conn:
                    logging.info('Connection from:' + addr)
                    while True:
                        data = conn.recv(1)
                        if not data:
                            conn.shutdown(socket.SHUT_RDWR)
                            conn.close()
                            return data_recvd.decode('UTF-8')
                        data_recvd += data
            except socket.timeout:
                self.sock.shutdown(socket.SHUT_RDWR)
                self.sock.close()
                self.setup()
