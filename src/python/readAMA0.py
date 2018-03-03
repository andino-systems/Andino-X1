import serial
ser=serial.Serial('/dev/ttyAMA0', 38400, timeout=30)
line=ser.readline()
print line

