from flask import jsonify
from flask import Flask
from flask import render_template
from flask import abort
from flask import request
app = Flask(__name__)

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

Temp = ['123','8765']
Press = []

@app.route('/')
def hello_world():
    return 'Hello, World!\n'

@app.route('/temp', methods=['GET', 'POST'])
def api_welcome():
    if request.method == 'POST':
        ser.write("GET_T".encode('utf-8'))
        x = ser.readline()
        Temp.append((x.decode('utf-8')).split("=")[1].split("_")[0])
    #if request.method == 'GET':
        
    
    return Temp
    

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404
