# NOP100

The **NOP100** project implements an NMEA 2000 interface module with
twin MikroBUS sockets which allow specialisation of function by the
addition of appropriate MikroBUS Click interface cards.

The motherboard is based on a Teensy 4.0 micro-controller supported by
a CAN interface, parasitic power supplies and a user configuration
interface.

**NOP100** firmware manages NMEA 2000 connectivity, user interaction
and module configuration and is easily extended by the addition of
firmware dedicated to operation of the installed MikroBus cards and
the integration their function with NMEA 2000.

```
NOP100/
  + hardware/            # Kicad designs for the NOP100 motherboard
  + firmware/            # NOP100 firmware
    + NOP100.cpp         # NOP100 main application 
    + defines.h          # Symbolic link to a module specialisation
    + definitions.h      #   "       "
    + includes.h         #   "       "
    + loop.h             #   "       "
    + setup.h            #   "       "
  + modules/             # Collection of folders for particular specialisations
    + NOP100-SIM/        # Specialisation defining an NMEA switch input module
      + defines.h
      + definitions.h
      + includes.h
      + loop.h
      + setup.h
    + another module specialisation/
    + ...
```

The following overview of NOP100-SIM (an NMEA 2000 switch input
module) based on NOP100 gives an overview of a NOP100 specialisation
which exploits MikroE 5981 'Click' modules to provide connection of
binary switches to the NMEA bus and notification of their state
information using PGN 127501.



using  a simple
inclusion-based architecture allows the firmware to be
extended to support


The project has a number of elements.

*  a re-usable generic purpose hardware and
firmware platformprovides a
[hardware design](./hardware/README.md)
and
[firmware](./firmware/README.md)
for a generic NMEA2000 interface module.

The NOP100 hardware provides two
[mikroBUS](https://www.mikroe.com/mikrobus)
expansion sockets which accept
[MikroE click]()
modules as providers of real-world interfaces.

The NOP100 firmware is extended through one or more module definitions
which support interaction with the installed hardware interfaces.