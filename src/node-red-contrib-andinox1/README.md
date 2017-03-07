 node-red-contrib-andinox1
====================

A [Node-RED][1] node that handles messages from the [Andino X1 Board][2].

![Andino X1 - Raspberry Pi on DIN Rail](https://andino.systems/wp-content/uploads/industrielle-raspberry-pi-erweiterung-andino-x1-small.png)

Install
-------

Run the following command in your Node-RED user directory - typically `~/.node-red`

    npm install node-red-contrib-andinox1

Usage
-----

Wire the ttyS0 to the input and set the filter to Counter-Events or Pin-Change-Events.
Wire the output to ttyS0, configure Relais1 or Relais2 and send true/false 0/1 to set the relais.

Example
-----
##### Flow
![Flow Example](http://andino.systems/wp-content/uploads/node-red-andinox1-flow.png)
##### UI
![Flow Example](http://andino.systems/wp-content/uploads/node-red-andinox1-ui.png)
##### Properties
![Flow Example](http://andino.systems/wp-content/uploads/node-red-andinox1-properties.png)

Author
-----

* 2017 by [AndinoSystems][3]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://nodered.org
[2]:https://andino.systems/andino-x1/
[3]:https://github.com/andino-systems/Andino-X1
