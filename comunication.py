import Comune_Ardu
import Sock_Serv
import time
import datetime
import json
import os
import logging

uno = Comune_Ardu.Comune_Ardu()

logging.basicConfig(level=logging.INFO)

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

while True:
	# sleep to give the processor some time
	# to do other tasks.
	time.sleep(.2)
	# Read data from serial line
	data = uno.read()
	logging.info("read data from uno: ", data)
	# key : value pairs are split by commas, this splits the string
	# into an array of strings that each holds a key : value pair
	split_data = data.split(',')

	# Create a unix time stamp. This produces an epoch in seconds as a floating
	# point.
	sensor_data['time_stamp'] = time.time()

	# looping through the key : value pairs
	for item in split_data:
		# splitting the key from the value
		tmp = item.split(':')
		if len(tmp) != 2:		# Checking that we have both a key, and a value
			print('Malformed data')
			break

		# Assigning respective key : value pairs
		if tmp[0] in sensor_data.keys():
			sensor_data[tmp[0]] = tmp[1]
			logging.info("values: %s, %s", tmp[0], tmp[1])

		# Changing ints to bools
		sensor_data['heat'] = bool(sensor_data['heat'])
		sensor_data['light_status'] = bool(sensor_data['light_status'])

	# will convert this all to logging soon, here for troubleshooting
	# purposes.
	print('Time:',
		datetime.datetime.fromtimestamp(sensor_data['time_stamp']).strftime(
		'%Y-%m-%d %I:%M:%S.%f %p'
		)
	)
	for item in sensor_data.keys():
		logging.info("%s, %s", item, sensor_data[item])

	# this loops through a list of expected keys, and creates a dictionary
	# by assigning the string in the list as a key, and eval actually gets
	# the variable name which is intentionally the same as the key and evalutes
	# it to get the data.

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
