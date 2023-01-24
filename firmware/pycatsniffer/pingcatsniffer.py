import serial
import time
import sys

ser = serial.Serial()
ser.port = '/dev/ttyACM0'
ser.baudrate = 921600
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 1

ping = bytearray([0x40, 0x53, 0x40, 0x00, 0x00, 0x40, 0x40, 0x45])	

time.sleep(1)
ser.open()

ser.write(cmd)

while(ser.in_waiting == 0):
	pass
		
time.sleep(0.01)
		
if ser.in_waiting > 0:
	msg = ser.read(ser.in_waiting)
	print (msg.hex())	
	      
ser.flush()
time.sleep(2)

ser.close()

