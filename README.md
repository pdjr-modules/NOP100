# NOP100

NOP100 provides a
[hardware design](./hardware/README.md)
and
[firmware](./firmware/README.md)
for a generic, abstract, NMEA2000 module that has no useful
purpose in the real-world.
If you connect a NOP100 module running this firmware then it will
appear on the host network as a device with Class Code 10 (System
Tools) and Function Code 130 (Diagnostic).

The NOP100 hardware and firmware designs are intended to form the
basis of real world NMEA devices and both can be easily extended
to make a plethora of useful things.
The following NMEA modules use **NOP100** as their underlying
architecture.

* [ROM106 - NMEA 2000 relay output module](https://www.github.com/preeve9534/ROM106)
* [SIM108 - NMEA 2000 switch input module](https://www.github.com/preeve9534/SIM108)
* [TMP106 - NMEA 2000 temperature sensor module](https://www.github.com/preeve9534/TMP106)

