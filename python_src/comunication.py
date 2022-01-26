import Comune_Ardu
import Sock_Serv
import time
import datetime
import json
import os
import logging

logging.basicConfig(level=logging.INFO)
logging.info(os.getcwd())

uno = Comune_Ardu.Comune_Ardu()
server = Sock_Serv.Server(location='/home/pi/PiGro/www/cgi/sock')

# comunication.py
#
# This script is intended to be run as a daemon service.
#
# It facilitates comunicaiton between the arduino Uno and the webserver
# to visualize sensor data from a greenhouse and to send desired watering,
# lighting, etc... thresholds to the arduino.
#
# It will loop forever constantly checking for comunication
# over the serial line from an ardiuno, and checking a UDS
# for data arriving from apace2 to send to the arduino.

# Default states to set sensor data values too.
sensor_data = {
	'temp': None,
	'light': None,
	'humidity': None,
	'time_stamp': None,
	'heat': None,
	'light_status': None,
	'soil_moisture_1': None,
	'soil_moisture_2': None
}

# Default states to set threshold data values too.
thresh_data = {
	'low_temp' : None,
	'high_temp' : None,
	'water_time' : None,
	'light_thresh' : None,
	'low_humidity' : None,
	'water_thresh' : None
}

while True:
	# check the UDS for data
	logging.info("Checking UDS")
	data = server.get_data()
	time.sleep(0.1)
	server.setup()

	if data is None:
		logging.debug('No data, moving on')
	else:
		logging.info("Data Received: " + data)
		# Writing data to uno
		uno.write(data)

	# Read data from serial line
	logging.info("Waiting for serial communication")
	data = uno.read()
	logging.info("read data from uno: " + data)

	# key : value pairs are split by commas, this splits the string
	# into an array of strings that each holds a key : value pair
	split_data = data.split(',')

	# Create a unix time stamp. This produces an epoch in seconds as a floating
	# point.
	sensor_data['time_stamp'] = time.time()
	logging.info("%s", datetime.datetime.fromtimestamp(sensor_data['time_stamp']))

	for item in split_data:
		tmp = item.split(':')
		if len(tmp) != 2:		# Checking that we have both a key, and a value
			print('Malformed data')
			break

		# Assigning respective key : value pairs
		if tmp[0] in sensor_data.keys():
			sensor_data[tmp[0]] = tmp[1]
			logging.info("values: %s, %s", tmp[0], tmp[1])
		elif tmp[0] in thresh_data.keys():
			thresh_data[tmp[0]] = tmp[1]
			logging.info("values: %s, %s", tmp[0], tmp[1])

	# Changing ints to bools
	sensor_data['heat'] = bool(int(sensor_data['heat']))
	sensor_data['light_status'] = bool(int(sensor_data['light_status']))

	# Saving data to JSON
	file_path = '/home/pi/PiGro/www/data/sensor_data.json'

	if os.path.isfile(file_path):
		with open(file_path) as jf:
			data = json.load(jf)
			data.append(sensor_data)
	else:
		data = [sensor_data]

	with open(file_path, 'w') as of:
		json.dump(data, of)

	# sleep to give the processor some time
	# to do other tasks.
	time.sleep(0.1)
