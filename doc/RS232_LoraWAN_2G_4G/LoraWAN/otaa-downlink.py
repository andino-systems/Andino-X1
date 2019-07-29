#!/usr/bin/python
# -*- encoding: utf-8 -*-
import time
import serial
import datetime

p = serial.Serial("/dev/ttyAMA0" , 57600 , timeout=5)

# Uplink message to send

up="aa"

writeconfig = 1

# Write to the RN2483 and if downlink is received, return it

def send(data):
   p.write(data+"\x0d\x0a")
   data.rstrip()
   print(data)
   time.sleep(2)

   down = None

   while True:
      rdata=p.readline()
      if rdata[:6] == "mac_rx":
         down=rdata[9:-1]
      else:
         print rdata[:-1]
      if rdata == "":
         break

   return down

# Reset the RN2483 module

send("sys reset")
time.sleep(1)

# Configure the network parameters

if writeconfig is 1:
   time.sleep(1)
   send("mac set appeui 70B3D57ED00143DD")
   send("mac set appkey 052CB5BC17B7773D3C2D9F52A6F8F65D")
   send("mac set adr off")
   send("mac save")
   time.sleep(5)

# Do an OTAA join

send("mac join otaa")

time.sleep(5)

# Send uplink and if we get downlink, print it out

while True:
   down = send("mac tx uncnf 1 "+up)

   if down:
      print "DOWNLINK: "+down

   time.sleep(10)
