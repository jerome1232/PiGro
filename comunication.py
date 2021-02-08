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

while True:
    data = uno.read()
    split_data = data.split(',')
    time_stamp = time.time()

    for item in split_data:
        tmp = item.split(':')
        if len(tmp) != 2:
            print('Malformed data')
            break
        if tmp[0] == 'light':
            light = tmp[1]
        elif tmp[0] == 'temp':
            temp = tmp[1]
        elif tmp[0] == 'humidity':
            humidity = tmp[1]

        print('Time:',
                datetime.datetime.fromtimestamp(time_stamp).strftime(
                    '%Y-%m-%d %H:%M'
                )
            )
        print('Temp: ', temp)
        print('Humidity: ', humidity)
        print('Light level: ', light)
        print()

        sensor_data = {}
        for var in ['time_stamp', 'temp', 'humidity', 'light']:
            sensor_data[var] = eval(var)

        data = None

        file_path = '/home/pi/PiGro/www/data/sensor_data.json'

        if os.path.isfile(file_path):
            with open(file_path) as jf:
                data = json.load(jf)
                data.append(sensor_data)
        else:
            data = [sensor_data]

        with open(file_path, 'w') as of:
            json.dump(data, of)
