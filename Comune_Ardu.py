#!/usr/bin/env python3
import serial
import time

class Comune_Ardu:
	'''Comune_Ardu Serial communication

	A class that facilitates serial communication
	with an arduino uno.

	By default it is assumed all communication begins with
	a "start" character and ends with a "end" character.

	Example input from Arduino expected:
		"<This is a single sentence.>"
		"<1,2,3,4,5>"
		"<Communication is happening>"

	Attributes:
		start_char		The character expected at the start of each full
							message.
		end_char		The chracter expected at the end of each full
							message.
		dev				The device path to the Arduino's serial device
		rate			The baud reate at which communication will
							occure.
		ser				A serial object to interact with the Arduino
	'''

	def __init__(
			self,
			start_char = None,
			end_char = None,
			dev = None,
			rate = None
		):
		'''Class constructor.'''
		self.start_char = '<' if start_char is None else start_char
		self.end_char = '>' if end_char is None else end_char
		self.dev = '/dev/ttyACM0' if dev is None else dev
		self.rate = 9600 if rate is None else rate

		self.ser = serial.Serial(self.dev, self.rate, timeout=None)

	def write(self, message):
		'''Simply encodes a string to bytes and writes it to serial line.'''
		self.ser.write(message.encode())

	def read(self):
		'''Reads one full message and returns it as a string.'''
		self.ser.flush()		# flush the buffer
		item = self.ser.read(1)	# Read a single character in from serial
		item = item.decode()	# Decode bytes object to UTF-8 string
		data = ''				# Initilize data to empty string

		# This loops until it finds a "start_char" to avoid
		# accidentally reading a half message.
		while item != self.start_char:
			item = self.ser.read(1)
			item = item.decode()

		# Once a start_char is found, loop until we find an end_char
		# I should probably also set some sort of upper limit to avoid
		# getting stuck looking for an end_char that is never comming.
		while item != self.end_char:
			if item != self.start_char:		# Don't actually add the start_character
				data = data + item			# Add the item to our data string
			item = self.ser.read(1)			# Read the next character
			item = item.decode()			# Decode it to UTF-8

		return data
