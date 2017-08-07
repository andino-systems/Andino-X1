Dual Channel UART RS232 Extension
========

This Plugin for the [Andino X1][1], provides a DualChannel UATRT to the Raspberry Pi or the Arduino Controller.

![Andino X1 - Dual Channel RS232 Extension](https://andino.systems/wp-content/uploads/rs232-extension-small.png)

Overview
====


The Board based on the SPI Uart from NXP [SC16IS752][3]. The SPI Channel can be jumpered to the Raspberry Pi or the Arduino Controller. With the Arduino Controller time critical protocols can be implemented or a general pre processing of the data can be performed.

![Andino X1 -Screw Terminal](https://andino.systems/wp-content/uploads/extension-connector-300x182.png)

![Andino X1 - RS232 Connector Skrew Terminal](https://andino.systems/wp-content/uploads/RS232-small.png)

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
