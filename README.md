# NOP100

NOP100 provides a
[hardware design](./hardware/README.md)
and
[firmware](./firmware/README.md)
for a generic, abstract, NMEA2000 module.

If you build the hardware and install the firmware then you will
have a device with no real-world application.
If you connect such a module to your network it will appear as a
device with Class Code 10 (System Tools) and Function Code 130
(Diagnostic).
You can interact with a NOP100 to the extent of setting its module
instance number.

That's it.

To build a module that performs some real-world task you will need
some supporting hardware and an elaborated NOP100 firmware that
exploits it.
Follow the links above to find out more.
