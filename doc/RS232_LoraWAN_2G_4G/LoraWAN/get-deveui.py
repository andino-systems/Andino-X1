#!/usr/bin/python
# -*- encoding: utf-8 -*-
import time
import serial

def send(data):
   p = serial.Serial("/dev/ttySC1" , 57600, rtscts=False )
   p.setRTS(False)
   time.sleep(2)
   p.write(data+"\x0d\x0a")
   data.rstrip()
   print(data)
   time.sleep(2)
   rdata=p.readline()
   rdata=rdata[:-1]
   print rdata

send("sys reset")

time.sleep(1)

send("mac get deveui")
