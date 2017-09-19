Fully isolated RS485 and RS422 Extension
========

This Plugin for the [Andino X1][1], provides a single channel, fully Isolated RS485 extension for the Raspberry Pi or the Arduino Controller. This PlugIn can be jumpered as a two wire, half duplex or as a four wire full duplex interface.

![Andino X1 - RS485 RS422 Plugin](https://andino.systems/wp-content/uploads/rs485-extension-small.png)

Overview
====


This Board based on the SPI Uart from NXP [SC16IS752][3]. The SPI Channel can be jumpered to the Raspberry Pi or the Arduino Controller. With the Arduino Controller time critical protocols can be implemented or a general pre processing of the data can be performed.
In Half Duplex Mode the transmitter can be switched on by the application with the RTS signal or automatically by the Hardware (Auto Transmitter on). 
#### Connector

![Andino X1 -Screw Terminal](https://andino.systems/wp-content/uploads/extension-connector-300x182.png)

![RS485 Connector screw terminal](https://github.com/andino-systems/Andino-X1/blob/master/doc/RS485/RS485_RS422.png?raw=true)

#### Jumper

![RS485 Jumper](https://andino.systems/wp-content/uploads/RS485-jumper.png)

#### Content
This Repository contains the 
* Schematics
* Overlay for the Raspberry Pi
* Library for the Arduino

#### Installation on the Raspberry Pi 

Install sc16is752 overlay from dts-file
```
wget https://github.com/andino-systems/Andino-X1/raw/master/doc/RS232/Raspberry/sc16is752-spi0.dts
wget https://github.com/andino-systems/Andino-X1/raw/master/doc/RS232/Raspberry/makedts.sh
chmod +x makedts.sh
./makedts
```

or download the dtbo file
```
wget https://github.com/andino-systems/Andino-X1/raw/master/doc/RS232/Raspberry/sc16is752-spi0.dtbo
sudo cp ./sc16is752-spi0.dtbo /boot/overlays/
```

* Append this to the /boot/config.txt
```
sudo nano /boot/config.txt
```
```
dtparam=spi=on
dtoverlay=spi0-cs,cs0_pin=8,cs1_pin=12
dtoverlay=sc16is752-spi0,int_pin=7
```
* Reboot. After that two new Devices are available /dev/ttySC0 and /dev/ttySC1

### Test
```
sudo apt-get install minicom
sudo minicom --setup
```
select /dev/ttySC0 or /dev/ttySC1

### Application examples
* Meter Reading (e.g. IEC 1107)
* Serial data Collect

### Usefull links
[Forum SC16IS752 (SC16IS7XX driver) Device Tree problem][4]
[Device Tree, Overlays and Parameters][5]
[List of Overlays][5]

Author
-----

* 2017 by [AndinoSystems][2]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://andino.systems/andino-x1/
[2]:https://github.com/andino-systems/Andino-X1
[3]:http://www.nxp.com/docs/en/data-sheet/SC16IS752_SC16IS762.pdf?
[4]:https://www.raspberrypi.org/forums/viewtopic.php?t=146908&p=1083915#p1024636
[5]:https://www.raspberrypi.org/documentation/configuration/device-tree.md
[6]:https://github.com/raspberrypi/firmware/tree/master/boot/overlays
