Andino X1
==========

[Andino X1][1], a base board that allows the raspberry pi to be used in an industrial environment

![Andino X1 - Raspberry Pi on DIN Rail](https://andino.systems/wp-content/uploads/industrielle-raspberry-pi-erweiterung-andino-x1-small.png)

## Overview

The [Andino X1][1] is a microcontroller board for the Raspberry Pi in a DIN-rail housing for installation in a control cabinet. It is used to adapt digital inputs and outputs for a voltage of 24 V. The X1 has its own microcontroller for precise signal preprocessing and adaptation of signal generators and actuators. It also contains a Raspberry Pi (2/3). The inputs and outputs as well as the power supply of the Pi are optimally protected. Communication between the microcontroller and the Pi takes place via the UART interface.

The [Andino X1][1] offers the following advantages:

The sensitive GPIO of the Raspberry Pi are protected. Fast signals can be precisely detected by the microcontroller. Actuators and sensors can be electrically connected to the Raspberry Pi. It provides an industrial power supply for the Raspberry Pi. Customized adapters from the Raspberry Pi GPIO or the micro controller IO can be connected electrically to terminals. Provides mounting on a DIN rail for installation in manifolds.

#### Raspberry Pi compatible 
The 40-pin connector is compatible with Raspberry Pi 2 B and Raspberry Pi 3

#### Arduino compatible 
The Atmel microcontroller of the Andino X1 comes with an Arduino-compatible bootloader. The combination of Arduino and Raspberry Pi on the Andino X1 is ideally suited for use in home automation and sensor technology, as well as in more demanding industrial automation applications. The strengths of both boards complement each other perfectly. While the single-board computer Raspberry Pi can perform complex tasks (eg hosting of database and WebServer) as a full-value computer, the Arduino microcontroller can take care of the fast signal pre-processing. The Atmel Controller communicates via UART with the Pi. Programmable is the X1 with the Arduino IDE via USB from a PC or from a Raspberry (firmware update in the field).

#### Integrated power supply 
The X1 board has a 9-24V wide-range DC input with reverse polarity protection. Powerful, reliable, stable power supply: 5 Volt, 2.6 Amp – enough power for the Raspberry, your USB hardware and customer-specific adaptation. The integrated EMC protection circuits protect the Pi from voltage surges and current surges on the supply line.

#### 8 Bit Microcontroller 
Programmable 8-bit microcontroller (Atmega 168 8Mhz) for adapting the inputs and outputs. Accurate and reliable detection of digital and analog signals.

#### Galvanically isolated 
The X1 board has two electrically isolated inputs (up to 5kV isolated) as well as two relay outputs for 42 volts and 1 amp. The IO is controlled by a microcontroller. Further GPIO of the Raspberry Pi as well as IO of the Microcontroller are led on an internal pin header. This makes it possible to bring own adaptations to the screw terminals.

#### Expandable
Via the SPI and the I2C interface of the Raspberry Pi, further hardware extensions can be connected and led to the free screw terminals. Thus, a stable, control cabinet-compatible wiring is possible. Further Interfaces: RS485 / RS422 (2/4 lines) (planed for Q1/2017) M-Bus (Meter Bus) Analog Input (e.g. via Microchip MCP3008) Digital Input

#### Realtime Clock (RTC)
The integrated, battery-buffered RTC provides the correct time even if no NTP (time) server is available. The high-precision time chip DS3231 from Dallas Semiconductors is used. Due to the internal temperature compensation of the oscillator, the chip achieves a very high accuracy of ± 2ppm at 0 ° C to + 40 ° C.

### Extendable
The SPI and I2C Busses of the Raspberry Pi and the Arduino Controller are connected with a Pin Header.
Different plugins can be plugged into this header. 
![Andino X1 - Extension port](https://andino.systems/wp-content/uploads/Extension-Pinout-300x293.png)
Different extensions are available.

#### Dualchannel RS232 Extension
Extends the Raspberry or the Arduino by two serial interfaces 

[![Andino X1 - Dualchannel RS232 Extension](https://andino.systems/wp-content/uploads/rs232-extension-small-300x205.png)](https://github.com/andino-systems/Andino-X1/tree/master/doc/RS232)

**See [here](https://github.com/andino-systems/Andino-X1/tree/master/doc/RS232) for more details.**

#### RS485 / RS422 Extension
Extends the Raspberry or the Arduino by a fully isolated RS485 / RS422 interface

[![Andino X1 - Dualchannel RS485 Extension](https://andino.systems/wp-content/uploads/rs485-extension-small-300x235.png)](https://github.com/andino-systems/Andino-X1/tree/master/doc/RS485)

**See [here](https://github.com/andino-systems/Andino-X1/tree/master/doc/RS485) for more details.**

#### 3 Digital Input
With this Extension the total count of Inputs is five. The Inputs are galvanically isolated to max 5kV.

[![Andino X1 - Digital input extension Extension](https://andino.systems/wp-content/uploads/3in-extension-small-300x211.png)](https://github.com/andino-systems/Andino-X1/tree/master/doc/3DI)

**See [here](https://github.com/andino-systems/Andino-X1/tree/master/doc/3DI) for more details.**

#### 1 Digital Input / 2 Relay Output
With this Extension the total count of Inputs is three and the total Relays are four. The Inputs are galvanically isolated to max 5kV.

[![Andino X1 - Digital input, Relais extension Extension](https://andino.systems/wp-content/uploads/1in2out-extension-small-300x209.png)](https://github.com/andino-systems/Andino-X1/tree/master/doc/1DI2DO)

**See [here](https://github.com/andino-systems/Andino-X1/tree/master/doc/1DI2DO) for more details.**

#### Breadboard
For prototyping of a additional extension a Breadboard is also available.

![Andino X1 - Breadboard Extension](https://andino.systems/wp-content/uploads/breadboard-extension-small-300x232.png)

### Application examples
* Data collection on production machines
* Collect and count Number of items, products
* Downtime detection
* Create Performance indicators Creation such as OEE, GAE and utilization
* Data collection at environmental monitoring stations
* Telecontrol and protocol converters
* Central in the house automation
* IoT nodes

Author
-----

* 2017 by [AndinoSystems][2]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://andino.systems/andino-x1/
[2]:https://github.com/andino-systems/Andino-X1

