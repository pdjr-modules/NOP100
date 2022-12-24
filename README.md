# NOP100

NOP100 provides a
[hardware design](./hardware/README.md)
and
[firmware](./firmware/README.md)
for a generic, abstract, NMEA2000 module.

If you build the hardware and install the firmware then you will
have a device with no real-world application (but see below!).

If you connect a NOP100 module running this firmware then it will
appear as a device with Class Code 10 (System Tools) and Function
Code 130 (Diagnostic).

You can interact with a NOP100 device to the extent of changing
its module instance number.

To build a module that performs some real-world task you will need
to extend the basic NOP100 hardware and extend the NOP100 firmware
that exploits it.

Follow the links above to find out more.
