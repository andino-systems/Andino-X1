 node-red-contrib-andinox1
====================

A [Node-RED][1] node that handles messages from the [Andino X1 Board][2].

![Andino X1 - Raspberry Pi on DIN Rail](https://clearsystems.de/wp-content/uploads/2016/09/Andino-X1-Set.png)

-------

# How to Install

## 1.) Some preperation

First of all you need Git

	sudo apt -y install git

Enable the communication to the Arduino via /dev/ttyAMA0. 
[Please see here: https://github.com/andino-systems/Andino-X1/tree/master/doc/BaseBoard](https://github.com/andino-systems/Andino-X1/tree/master/doc/BaseBoard)


## 2.) Node Red for Raspberry Pi

### 2.1) Usefull extensions
To save write cycles on your SD Card you can install [Log2Ram](https://github.com/azlux/log2ram) which stores the Log Files in memory.

	sudo apt -y install git
	git clone https://github.com/azlux/log2ram.git
	cd log2ram
	chmod +x install.sh
	sudo ./install.sh
	sudo reboot now 

check if its running

	df -h
	log2ram          40M  452K   40M   2% /var/log


After that you can install Node Red
First Node.js

	bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered) 

run Node Red from terminal

	node-red-pi

To run Nore-Red in background on boot:

	 sudo systemctl enable nodered.service


## 3.) Node for Andino X1

Run the following command in your Node-RED user directory - typically `~/.node-red`

    npm install node-red-contrib-andinox1

What this Node does is
1.) Convert the Serial Message vom the Arduino Controller to an Payload Object so that you can access payload.Counter1 for instance.
2.) Convert a Boolean to a Relay-Message for the Microcontroller (see the Example)  

From the Controller

	msg.payload : string[23]
	":0664{0026,0016}{1,0}↵"

After the Andino Node

	msg.payload : Object
	{ Counter1: 38, Counter2: 22, Pin1: 1, Pin2: 0 }

Usage
-----

Wire the ttyAMA to the input and set the filter to Events.
Wire the output to ttyAMA0, configure Relais1 or Relais2 and send true/false 0/1 to set the relais.

### Example
-----

#### Flow
![Node Red Example](http://andino.systems/wp-content/uploads/node-red-andinox1-flow.png )

-----

#### Dashboard
![Node Red Dashboard](http://andino.systems/wp-content/uploads/node-red-dashboard.png)

-----

#### Properties
![Node Red - Andino X1 Properies](http://andino.systems/wp-content/uploads/node-red-andinox1-properties.png)

-----

#### Output Payload
![Node Red - Andino X1 Payload](http://andino.systems/wp-content/uploads/node-red-payload-output.png)

-----


#### Serial Settings
![Node Red - Andino X1 Serial Settings](https://andino.systems/wp-content/uploads/node-red-andinox1-serial.png)
----
Some infos and Samples about Node Red

https://randomnerdtutorials.com/getting-started-with-node-red-on-raspberry-pi/

Author
-----

* 2018 by [AndinoSystems][3]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://nodered.org
[2]:https://andino.systems/andino-x1/
[3]:https://github.com/andino-systems/Andino-X1
