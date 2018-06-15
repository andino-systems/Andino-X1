# Andino X1

Homepage of [Andino X1](https://andino.systems/andino-x1/)

Tabe of Content   
   
1. [How to enable the UART](README.md#how-to-enable-the-uart)
2. [Default Firmware on the Atmel](README.md#default-firmware-on-the-atmel)

------

## How to enable the UART

### 1. Enable UART as /dev/ttyAMA0 or /dev/serial0 

	sudo nano /boot/config.txt

add this at the end of the file..

	enable_uart=1
	dtoverlay=pi3-disable-bt-overlay 
	dtoverlay=pi3-miniuart-bt

### 2. Disable Console on /dev/serial0

	sudo nano /boot/cmdline.txt

Remove “console=… (cut between ~~)
  
	dwc_otg.lpm_enable=0 ~~console=serial0,115200 console=tty1~~ root=/dev/mmcblk0p2 rootfstype=ext4 .....

### 3. Reboot and test

	sudo apt-get install minicom
	sudo minicom --setup

Set Hardware Flow to No, set Device to /dev/ttyAMA0 or /dev/serial0, set BPS to 38400

	+-----------------------------------------------------------------------+
	| A - Serial Device : /dev/ttyAMA0                                      |
	| B - Lockfile Location : /var/lock                                     |
	| C - Callin Program :                                                  |
	| D - Callout Program :                                                 |
	| E - Bps/Par/Bits : 38400 8N1                                          |
	| F - Hardware Flow Control : No                                        |
	| G - Software Flow Control : No                                        |
	|                                                                       |
	| Change which setting?                                                 |
	+-----------------------------------------------------------------------+
	 | Screen and keyboard      |
	 | Save setup as dfl        |
	 | Save setup as..          |
	 | Exit                     |
	 | Exit from Minicom        |
	 +--------------------------+

## Default Firmware on the Atmel

You will find the description and the INO-File of the default Firmware here:

[https://github.com/andino-systems/Andino-X1/tree/master/src/firmware/counting](https://github.com/andino-systems/Andino-X1/tree/master/src/firmware/counting)

The default UART Settings are 38400 / 8 / n / 1

## Programm your own Firmware

1. First install the Aruino IDE on your PC.
2. Set the Arduino IDE to the currect Board
2. Set the Jumper of the Andino to USB.
3. Programm the Firmware and test it.
4. Set the Jumper back to Raspberry Pi mode

