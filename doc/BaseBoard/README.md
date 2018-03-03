How to enable the UART
==========

#### 1. Enable UART as /dev/ttyAMA0 or /dev/serial0 

```sh
sudo nano /boot/config.txt
```
add this at the end of the file..

```
enable_uart=1
dtoverlay=pi3-disable-bt-overlay 
dtoverlay=pi3-miniuart-bt
```

#### 2. Disable Console on /dev/serial0
```sh
sudo nano /boot/cmdline.txt
```
Remove “console=…  
```
dwc_otg.lpm_enable=0 ~~console=serial0,115200 console=tty1~~ root=/dev/mmcblk0p2 rootfstype=ext4 .....
```
#### 3. Reboot and test

```
sudo apt-get install minicom
sudo minicom --setup
```
Set Hardware Flow to No, set Device to /dev/ttyAMA0 or /dev/serial0, set BPS to 38400
```
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
```
