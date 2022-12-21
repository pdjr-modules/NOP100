# pdjr-kicad-nmea2000-common

This project provides a circuit design and PCB layout for the generic
elements of an NMEA 2000 module based around the Teensy 4.0
microcontroller.

The PCB layout can be imported into other projects, relieving them of
the effort of designing and implementing fundamental services which
power, interface, configure and process most NMEA hardware devices.
A project designer using the PCB layout can focus on just those
elements of application design appropriate to their task in hand.

A parallel project
[pdjr-nmea2000-common-firmware]()
provides a C++ pattern and associated libraries that can be used as
a basis for implementing firmware which exploits this hardware.

## Design elements

### PCB

The double-sided PCB is suitable for use in a *** case and employs
both surface-mount and through-hole components.

### Microcontroller

The project supports both Teensy 3.2 and Teensy 4.0 microcontroller
modules.

### Power supply

A TMR-2-2411 DC-DC converter takes bus power from NET-S/NET-C and
outputs a 5VDC 400mA supply for the PCB.

The converter power supply input is protected by a 0.25A
self-resetting polymer fuse selected on the assumption of a 12VDC
bus voltage.

Output from the converter is filtered through a smooting capacitor.

### CAN interface

An MCP-2551/IP CAN controller converts data on CAN-H/CAN-L to
signals suitable for microcontroller use. The signals are
level-shifted to 3.3VDC for compatability with the Teensy 4.0.

A switch switch-selectable 120 Ohm bus termination resistor allows
the module to be used as either an NMEA drop or terminal node.

### Configuration interface

The configuration interface consists of an SPST momentary tactile
switch (PRG) and an 8-position DIL switch (INSTANCE).

PRG is directly connected to GPIO D14 and is active low (GPIO D14
should be configured with INPUT_PULLUP).

INSTANCE channels are pulled low, active high, and connected to the
parallel inputs of a PISO buffer interfaced through GPIO D10, D11
and D12.

### Display interface

The display interface consists of a single TRANSMIT_LED and up to 16
additional status LEDs for use by the host application.

TRANSMIT_LED is driven directly by GPIO D15.

The status LEDs are driven at 5VDC by a SIPO buffer with the buffer
interface connected to GPIO D0, D1 and D2.
   
## How to use this PCB design as the basis for a new project

The PCB design is simply imported into a new KiCad project and then
extended to provide the required application features.

1. Open Kicad(1) and create a new project.

2. Download MNEA2000-COMMON.kicad_pcb into the new project folder.

3. Open pcbnew(1), choose FILE->MERGE, select the file you just
   downloaded and save it as a the new project's PCB (if your KiCad
   project is called blob.prj then save the file as blob.kicad_pcb).
   
4. 
