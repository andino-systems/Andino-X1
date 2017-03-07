 node-red-contrib-andinox1
====================

A <a href="http://nodered.org" target="_new">Node-RED</a> node that handles messages from the <a href="https://andino.systems/andino-x1/" target="_new">Andino X1 Board</a>.

Install
-------

Run the following command in your Node-RED user directory - typically `~/.node-red`

    npm install node-red-contrib-andinox1


Usage
-----

Wire the ttyS0 to the input and set the filter to Counter-Events or Pin-Change-Events.
Wire the output to ttyS0, configure Relais1 or Relais2 and send true/false 0/1 to set the relais.