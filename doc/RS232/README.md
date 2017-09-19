Dual Channel UART RS232 Extension
========

This Plugin for the [Andino X1][1], provides a DualChannel UATRT to the Raspberry Pi or the Arduino Controller.

![Andino X1 - Dual Channel RS232 Extension](https://andino.systems/wp-content/uploads/rs232-extension-small.png)

Overview
====


The Board based on the SPI Uart from NXP [SC16IS752][3]. The SPI Channel can be jumpered to the Raspberry Pi or the Arduino Controller. With the Arduino Controller time critical protocols can be implemented or a general pre processing of the data can be performed.

![Andino X1 -Screw Terminal](https://andino.systems/wp-content/uploads/extension-connector-300x182.png)

![Andino X1 - RS232 Connector Skrew Terminal](https://github.com/andino-systems/Andino-X1/blob/master/doc/RS232/RS232.png?raw=true)

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
