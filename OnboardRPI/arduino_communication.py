from serial import Serial
from time import sleep
from wifi import

ser = Serial('/dev/ttyUSB0',9600)
ser.write('p')
while True:
	read_serial=ser.readline()
	if (read_serial[:9] == "__RPI__: "):
		msg = read_serial[9:]
		ssid = msg.split()[0]
		passwd = msg.split()[1]
		exit()
	sleep(100/1000)
