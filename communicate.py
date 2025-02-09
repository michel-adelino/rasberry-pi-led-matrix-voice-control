import serial
import time
#ser = serial.Serial('/dev/ttyUSB0', 9600)
ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(5)
ser.write('4'.encode())
#ser.write(b'2')
time.sleep(2)
ser.write('5'.encode())
time.sleep(2)
ser.write('6'.encode())

msg = ser.read(ser.inWaiting())
print(msg)
