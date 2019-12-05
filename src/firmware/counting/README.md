Standard Andino X1 Firmware
==========

[Andino X1][1], a base board that allows the raspberry pi to be used in an industrial environment.

Overview
====

This example counts impulses at the digital inputs. 
Count stops are cyclically sent to the Raspberry. 
The digital inputs are additionally de-bounced. 
On the other hand, the relays can be switched or pulsed. The settings of the firmware are changed via commands and are stored in the eeprom.
This Version consider the extension boards [3DI][4] and [1DI2DO][5] (Command HARD).
  
The Communication runs with **38400 Baud**.

This Sample needs the [TimerOne][3] library.

### Interface, Commands

All commands or messages are sent and received via  /dev/ttyS0.
Every command has to be terminated by CR or LF. Message ends with CR and LF.
### Commands to the Controller
**Command** | Arguments | Action | Example 
--- | --- | --- | ---
RESET | none | Restart the Controller | RESET
INFO | none| Prints the current settings | INFO
HARD | 0=noShield, 1=1DI2DO, 2=3DI, 3=5DI | Set the Hardware configuration | 0 - none
POLL | Cycle in ms | Sets the sampling cycle of the digital inputs [in ms] | POLL 1000
SKIP | Number of polls | Skip n Scans after pulse reconized | 0
EDGE | HL(0) LH(1) | Count on edge HL or LH | EDGE
SEND | Cycle in ms | The counter will send all nnn milliseconds | SEND 5000
CHNG | Send on Change | Send if any Pin changes. Carefull if many changes | CHNG 1
CNTR | Send Counter | Send counter+states(1) or only states(0) (default 1) | CNTR 0
DEBO | Number of polls | Sets the debounce count. The signal has to be stable for nn polls | DEBO 100
POWR | state (0 or 1)| Power-Out Relay is switched on or off | REL1 1
REL1 | state (0 or 1)| Relay 1 is switched on or off | REL1 1
REL2 | state (0 or 1)| Relay 2 is switched on or off | REL2 1
REL3 | state (0 or 1)| Relay 3 is switched on or off | REL3 1
RPU1 | pulse in sec | Pulse the Relay 1 for nns seconds | RPU1 2
RPU2 | pulse in sec | Pulse the Relay 2 for nns seconds | RPU2 2

### Messages from the Controller

:Message-ID{counter1,counter2,..}{state1,state2}

The Message starts with a ':'. After that follows a Message-ID. This is a modulo HEX Counter from 0..FFFF.    
Then within a '{'  '}' the counter follows. The number of counter depends on the Hardware shields.   
The Counter are HEX encoded and runs from 0 to FFFF (modulo).  
Then again within a '{'  '}' the current state of the inputs follows. 0-off, 1-on.  
The number depends on the Hardware shields.
The Message ends with a CR / LF [0x0d + 0x0a]

Example   
:0040{0002,0000,000B}{0,0,0}  
:0041{0002,0000,000B}{0,0,0}  
:0042{0004,0000,000C}{0,0,0}  
:0043{0004,0000,000C}{0,0,0}  
:0044{0008,0000,000F}{0,0,1}  
:0045{0008,0000,000F}{0,0,1}  
:0046{000A,0000,0000}{0,0,1}  
:0047{000C,0000,0000}{1,0,0}  
:0048{0010,0000,0000}{0,0,0}  


Download
-----

Please see in the GIT Content the current INO File: [Version002.ino](Version002.ino)  


Author
-----

* 2017 by [AndinoSystems][2]
* [Contact us by email](mailto:info@andino.systems)

[1]:https://andino.systems/andino-x1/
[2]:https://github.com/andino-systems/Andino-X1
[3]:https://code.google.com/archive/p/arduino-timerone/downloads
[4]:https://github.com/andino-systems/Andino-X1/tree/master/doc/3DI
[5]:https://github.com/andino-systems/Andino-X1/tree/master/doc/1DI2DO
