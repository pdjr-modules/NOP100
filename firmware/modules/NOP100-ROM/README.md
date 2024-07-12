# NOP100-ROM - NMEA 2000 relay output module

NOP100-ROM is a 6-channel NMEA 2000 relay output module based on 
[NOP100]()
hardware and firmware.

NOP100-ROM exploits the
[MikroE 5981 Relay 5 Click]()
module as its external switch interface.
One or two modules can be plugged into the NOP100's MikroBus sockets
giving a maximum of 6 output channels.

NOP100-ROM firmware is NOP100 firmware extended by the code in this
folder which can be linked into the NOP100 source code by executing
the following command in the ```NOP100/firmware``` folder.
```
$> ./link-module NOP100-ROM
```
Subsequently, compiling code in ```NOP100/firmware/src``` folder will
generate NOP100-ROM firmware.