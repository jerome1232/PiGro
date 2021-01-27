#!/usr/bin/env python3
import serial
import time

class Comune_Ardu:
    '''
    A class that facilitates serial communication
    with an arduino uno
    '''

    def __init__(self):
        self.start_char = '<'
        self.end_char = '>'
        self.dev = '/dev/ttyACM0'
        self.rate = 9600
        self.ser = serial.Serial(self.dev, self.rate, timeout=1)

    def write(self, message):
        self.ser.write(message.encode())

    def read(self):
        self.ser.flush()
        item = self.ser.read()
        item = item.decode()
        data = ''

        # If there is not start char
        # we started reading mid message.
        # read until we get to a start char.
        while item != self.start_char:
            item = self.ser.read()
            item = item.decode()

        # Once we get to a start char
        # read until we hit an end_char
        while item != self.end_char:
            if item != self.start_char:
                data = data + item
            item = self.ser.read()
            item = item.decode()

        return data


comune = Comune_Ardu()

temp = 0
light = 0
humidity = 0

while True:
    data = comune.read()
    split_data = data.split(',')

    for item in split_data:
        tmp = item.split(':')
        if tmp[0] == "light":
            light = tmp[1]
        elif tmp[0] == "temp":
            temp = tmp[1]
        elif tmp[0] == "humidity":
            humidity = tmp[1]

    print("Light level: ", light)
    print("Temperature: ", temp)
    print("Humidity: ", humidity)
    print()
    print()




