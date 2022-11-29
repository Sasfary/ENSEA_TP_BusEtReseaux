#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial
import time

ser = serial.Serial(

	port='/dev/ttyAMA0',
	baudrate=115200,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=0xFF
)



while 1:
	ser.write("GET_P".encode('utf-8'))
	x = ser.readline()
	print (x.decode('utf-8') + "\r\n")
	time.sleep(10)
	print("sleep out\r\n")