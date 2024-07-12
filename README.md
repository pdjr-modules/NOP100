# NOP100

The **NOP100** project aims to develop a range of NMEA 2000 interface
modules based upon a micro-controller motherboard and a range of
MikroBUS 'click' interface cards.

The motherboard consists of a Teensy 4.0 micro-controller supported by
a CAN interface, a parasitic power supply connected to the NMEA bus, a
user configuration interface and two MikroBus host sockets.

An extensible firmware manages NMEA 2000 connectivity, user interaction
and module configuration.

A NOP100 device is implemented by selecting MikroBus 'Click' modules
which provide the necessary real-world interfaces and extending the
core firmware to operate the MikroBus modules and integrate their
operation with NMEA 2000.

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