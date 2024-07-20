# NOP100-SIM - NMEA 2000 switch input module

NOP100-SIM is an NMEA 2000 switch input module which uses the
[MikroE-5981 Digi Isolator 2 Click]()
module as its external switch interface.
One or two 5981 modules can be plugged into the NOP100's MikroBus
sockets giving either 8 or 16 input channels.

NOP100-SIM firmware extends NOP100 firmware with code in this
folder which can be linked into the NOP100 source code by executing
the following command in the ```NOP100/firmware``` folder.
```
$> ./link-module NOP100-SIM
```

Subsequently, compiling code in ```NOP100/firmware/src``` folder will
generate NOP100-SIM firmware.

## NMEA interface

**NOP100-SIM** broadcasts a PGN 127501 Binary Switch Status message
once every two seconds or immediately when a state change is detected
on a switch input on any of the connected MikroE 5891 modules.