# NOP100-SIM - NMEA 2000 switch input module

NOP100-SIM is an NMEA 2000 switch input module based on
[NOP100]()
hardware and firmware.
The module broadcasts the status of its external switch inputs over
the host NMEA bus using PGN 127501 Binary Status Report messages.

NOP100-SIM exploits the
[MikroE-5981 Digi Isolator 2 Click]()
module as its external switch interface.
One or two modules can be plugged into the NOP100's MikroBus sockets
giving either 8 or 16 input channels.

NOP100-SIM firmware is NOP100 firmware extended by the code in this
folder which can be linked into the NOP100 source code by executing
the following command in the ```NOP100/firmware``` folder.
```
$> ./link-module NOP100-SIM
```

Subsequently, compiling code in ```NOP100/firmware/src``` folder will
generate NOP100-SIM firmware.