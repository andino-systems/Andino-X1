```sh
sudo apt-get install python
sudo apt-get install python-pip
pip install pyserial
```

Create file 'readAMA0.py'
```python
import serial
ser=serial.Serial('/dev/ttyAMA0', 38400, timeout=30)
line=ser.readline()
print line
```

```sh
 python readAMA0.py
```