'''
UART communication on Raspberry Pi using Pyhton
http://www.electronicwings.com
'''
import serial
from time import sleep

ser = serial.Serial("/dev/ttyS0", 19200)    #Open port with baud rate
while True:
    received_data = ser.read()              #read serial port
    sleep(0.03)
    data_left = ser.inWaiting()             #check for remaining byte
    received_data += ser.read(data_left)
    print (received_data)                   #print r=eceived data
    ser.write(received_data)                #transmit data serially