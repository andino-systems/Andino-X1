Fully isolated RS485 and RS422 Extension
========

This Plugin for the [Andino X1][1], provides a single channel, fully Isolated RS485 extension for the Raspberry Pi or the Arduino Controller. This PlugIn can be jumpered as a two wire, half duplex or as a four wire full duplex interface.

![Andino X1 - RS485 RS422 Plugin](https://andino.systems/wp-content/uploads/rs485-extension-small.png)

Overview
====


This Board based on the SPI Uart from NXP [SC16IS752][3]. The SPI Channel can be jumpered to the Raspberry Pi or the Arduino Controller. With the Arduino Controller time critical protocols can be implemented or a general pre processing of the data can be performed.
In Half Duplex Mode the transmitter can be switched on by the application with the RTS signal or automatically by the Hardware (Auto Transmitter on). 
#### Connector
![RS485 Connector screw terminal](https://andino.systems/wp-content/uploads/RS485_RS422-small.png)
#### Jumper
![RS485 Jumper](https://andino.systems/wp-content/uploads/RS485-jumper.png)

#### Content
This Repository contains the 
* Schematics
* Overlay for the Raspberry Pi
* Library for the Arduino

#### Installation on the Raspberry Pi 
* Copy the dtbo files to /boot/
* Append this to the /boot/config.txt
```
dtparam=spi=on
dtoverlay=spi0-cs,cs1_pin=8  
dtoverlay=sc16is752-spi1
```
* Reboot. After that two new Devices are available /dev/ttySC0 and /dev/ttySC1

#### Installation on Arduino
Copy the content of Arduino\SPIUart to your Libaries Folder (C:\Users\[USERNAME]\Documents\Arduino\libraries.

### Application examples
* Meter Reading (e.g. IEC 1107)
* Serial data Collect


Author
-----

* 2017 by [AndinoSystems][2]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://andino.systems/andino-x1/
[2]:https://github.com/andino-systems/Andino-X1
[3]:http://www.nxp.com/docs/en/data-sheet/SC16IS752_SC16IS762.pdf?
