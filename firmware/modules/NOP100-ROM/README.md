# NOP100-ROM - NMEA 2000 relay output module

NOP100-ROM is a 6-channel NMEA 2000 relay output module based on 
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
hardware and firmware.

NOP100-ROM exploits the
[MikroE 5981 Relay 5 Click]()
module as its external switch interface.
One or two modules can be plugged into the NOP100's MikroBus sockets
giving a maximum of 6 output channels.

NOP100-ROM firmware can be linked into the NOP100 source code by
executing the following command in the ```NOP100/firmware``` folder.
```
$> ./link-module NOP100-ROM
```
Subsequently, compiling code in ```NOP100/firmware/src``` folder will
generate NOP100-ROM firmware.

## NMEA interface

**NOP100-ROM** broadcasts a PGN 127501 Binary Switch Status message
once every two seconds or immediately when a state change is detected
on a relay on any of the connected MikroE 5675 modules.

**NOP100-ROM** listens for PGN 127502 messages and updates relays on
connected MikroE 5675 modules to reflect the commanded states.