# NOP100-SIM - NMEA 2000 switch input module

This project provides the firmware needed to implement a 16-channel
NMEA 2000 switch input module based on the
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
architecture.

The underlying hardware platform must consist of a
[NOP100 motherboard](https://www.github.com/pdjr-n2k/NOP100/hardware)
with twin
[MikroE-5981 Digi Isolator 2 Click]()
expansion modules.

This firmware can be linked into the NOP100 source code by executing
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