# NOP100

The **NOP100** project implements an
[NMEA 2000](https://en.wikipedia.org/wiki/NMEA_2000)
interface module with twin
[MikroBUS](https://www.mikroe.com/mikrobus)
sockets which allow specialisation of function by the addition of
[Click](https://www.mikroe.com/click) interface cards.

[**NOP100** hardware](./hardware/README.md)
is based on the Teensy 4.0 micro-controller supported by a CAN
transceiver, parasitic power supplies and a user configuration
interface.

[**NOP100** firmware](./firmware/README.md)
manages NMEA 2000 connectivity, user interaction and module
configuration and is easily extended by the addition of firmware
dedicated to operation of the installed MikroBus cards and the
integration of their function with NMEA 2000.

## Project organisation

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