Andino X1 Firmware Sample – Counting Events
==========

[Andino X1][1], a base board that allows the raspberry pi to be used in an industrial environment.

![Andino X1 - Raspberry Pi on DIN Rail](https://andino.systems/wp-content/uploads/industrielle-raspberry-pi-erweiterung-andino-x1-small.png)

Overview
====

This example counts impulses at the digital inputs. Count stops are cyclically sent to the Raspberry. The digital inputs are additionally de-bounced. On the other hand, the relays can be switched or pulsed. The settings of the firmware are changed via commands and are stored in the eeprom.

This Sample needs the [TimerOne][3] library.

### Interface, Commands

All commands or messages are sent and received via  /dev/ttyS0.
Every command has to be terminated by CR or LF. Message ends with CR and LF.
### Commands to the Controller
**Command** | Arguments | Action | Example 
--- | --- | --- | ---
RESET | none | Restart the Controller | RESET
INFO | none| Prints the current settings | INFO
POLL | Cycle in ms | Sets the sampling cycle of the digital inputs [in ms] | POLL 1000
EDGE | HL(0) LH(1) | Count on edge HL or LH | EDGE
SEND | Cycle in ms | The counter will send all nnn milliseconds | SEND 5000
DEBO | Number of polls | Sets the debounce count. The signal has to be stable for nn polls | DEBO 100
REL1 | state (0 or 1)| Relay 1 is switched on or off | REL1 1
REL2 | state (0 or 1)| Relay 2 is switched on or off | REL2 1
RPU1 | pulse in sec | Pulse the Relay 1 for nns seconds | RPU1 2
RPU2 | pulse in sec | Pulse the Relay 2 for nns seconds | RPU2 2

### Messages from the Controller
**Message** | Arguments | Action | Example 
--- | --- | --- | ---
CNTR | messageID counter1 counter2 | Send the counter every [SEND] millis | CNTR 9998 12 32
STAT | messageID pin1 pin2 | Send the state of the pins every [SEND] millis | CNTR 9998 12 32

Message-ID increments on every Message to check any lost.


Author
-----

* 2017 by [AndinoSystems][2]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://andino.systems/andino-x1/
[2]:https://github.com/andino-systems/Andino-X1
[3]:https://code.google.com/archive/p/arduino-timerone/downloads