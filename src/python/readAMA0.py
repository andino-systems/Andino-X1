import serial
ser=serial.Serial('/dev/ttyAMA0', 19200, timeout=30)
line=ser.readline()
print line

