import Comune_Ardu
import Sock_Serv
import time
import datetime
import json
import os

uno = Comune_Ardu.Comune_Ardu()

temp = None
light = None
humidity = None
time_stamp = None
heat = None
light_status = None
soil_moisture_1 = None
soil_moisture_2 = None

while True:
	# sleep to give the processor some time
	# to do other tasks.
	time.sleep(.2)
	# Read data from serial line
	data = uno.read()
	# key : value pairs are split by commas, this splits the string
	# into an array of strings that each holds a key : value pair
	split_data = data.split(',')

	# Create a unix time stamp. This produces an epoch in seconds as a floating
	# point.
	time_stamp = time.time()

	# looping through the key : value pairs
	for item in split_data:
		# splitting the key from the value
		tmp = item.split(':')
		if len(tmp) != 2:		# Checking that we have both a key, and a value
			print('Malformed data')
			break
		# assigning appropriate keys found to their respective variables.
		if tmp[0] == 'light':
			light = tmp[1]
		elif tmp[0] == 'temp':
			temp = tmp[1]
		elif tmp[0] == 'humidity':
			humidity = tmp[1]
		elif tmp[0] == 'heat':
			heat = True if int(tmp[1]) == 1 else False
		elif tmp[0] == 'light_status':
			light_status = True if int(tmp[1]) == 1 else False
		elif tmp[0] == 'soil_moisture_1':
			soil_moisture_1 = tmp[1]
		elif tmp[0] == 'soil_moisture_2':
			soil_moisture_2 = tmp[1]

	# will convert this all to logging soon, here for troubleshooting
	# purposes.
	print('Time:',
		datetime.datetime.fromtimestamp(time_stamp).strftime(
		'%Y-%m-%d %I:%M:%S.%f %p'
		)
	)
	print('Temp: ', temp)
	print('Humidity: ', humidity)
	print('Light level: ', light)
	print('Heater on?: ', heat)
	print('Lights on?: ', light_status)
	print()

	# this loops through a list of expected keys, and creates a dictionary
	# by assigning the string in the list as a key, and eval actually gets
	# the variable name which is intentionally the same as the key and evalutes
	# it to get the data.
	sensor_data = {}
	for var in [
			'time_stamp',
			'temp',
			'humidity',
			'light',
			'heat',
			'light_status',
			'soil_moisture_1',
			'soil_moisture_2'
		]:
		sensor_data[var] = eval(var)

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
