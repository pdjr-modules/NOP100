# NOP100/firmware

```NOP100.cpp``` is a complete, runnable, firmware for
[NOP100 hardware](../hardware/README.md).

The firmware is self-contained and if compiled and installed on
NOP100 hardware will create an NMEA 2000 device with Class Code 10
(System Tools) and Function Code 130 (Diagnostic). You can
interact with NOP100 firmware by entering arbitrary configuration
settings, but only the default module instance setting is actually
used by NOP100.

To build a module that performs some real-world task you will need
some supporting hardware and an elaborated NOP100 firmware that
exploits it.

The housekeeping and services implemented in the core NOP100 firmware
should be preserved and in an effort to encourage this firmware code is
implemented so that the application specific functionality required to
implement a particular module is abstracted out into a small
collection of include files which allow even quite complex behaviours
to be built with a minimum of code.

## Module configuration

NOP100 treats persistent configuration data as a simple byte array and
provides a user interface which allows the hardware DIL switch to be  
used sequentially to specify a storage address and the value to be
stored at that address.
This restricts the configuration data cache to a size of 256 bytes with
254 bytes available to the application.
The Teensy microcontroller has a about 1KB of EEPROM available and if
you want to access more than 256 bytes of this storage then you will
need to override NOP100's simple configuration model with a more
elaborate one of your own.
If you do this, make sure that your application configuration data
leaves address locations 0 and 1 for use by NOP100.

| Address | Saved value |
| ---:    | :---        |
| 0       | CAN source address. Not available for application use. |
| 1       | Module instance number. Not available for application use. |
| 2...    | Available for application use. |

Entry of configuration data into NOP100 requires a value to be set on
the PCB's DIL switch and then confirmed by press and release of the PRG
button.
A short press of PRG signifies the entry of a data value; a long press
of PRG signifies entry of an address and, in this latter case, the
expectation is that address entry will promptly be followed by the
entry of a data value which should be stored at the pre-set address.

If a value is entered without a preceeding address then the entered
value is stored at address 1 and will become the module's new instance
number.

Storage locations with an address greater than or equal to two are
intended for application configuration data and entry of this data
requires the entry of an address followed by entry of the value to be
stored at this address.

When an address is entered, the transmit LED will flash rapidly
indicating that the system is waiting for entry of a data value.
If a data value is not entered within one minute the data entry protocol
will self-cancel (the transmit LED will stop flashing).

In ```NOP100.cpp``` the configuration process is handled by the
```prgButtonHandler(bool _state_, int _value_)``` function which is
called each time the PRG button is operated.
*state* will be true if the PRG button has been released or false if
the PRG button has been pressed.
In either case, *value* will be set to the current value read from the
DIL switch.

You can override NOP100's built-in behaviour by overriding the button
handler in ```module-declarations.inc```.
For example, the following code disables configuration entirely:
```
#define PRG_BUTTON_HANDLER
void prgButtonHandler(bool state, int value) {
   return;
}
```

## HOW TO

1. Create parent folder for your new application.
   ```
   $> mkdir n2k-projects
   $> cd n2k-projects
   ```
2. Clone a copy of the Arduino firmware build project and this
   project into separate sub-folders.
   ```
   $> git clone https://github.com/preeve9534/firmware-factory
   $> git clone https://github.com/preeve9534/NOP100
   ```
3. Make a directory for your new project and populate it.
   ```
   $> mkdir myapp
   $> cd myapp
   $> cp ../NOP100/NOP100.cpp myapp.cpp
   $> cp ../NOP100/*.inc .
   $> cd ..
   ```
   You can now delete the NOP100 project.
   ```
   rm -rf NOP100
   ```
4. Set up the build environment so that it points to your new
   project.
   ```
   $> cd firmware-factory/sketch
   $> rm src
   $> ln -s ../../myapp src
   $> cd ../..
   ```
   At this juncture you should be able to open the firmware-factory 
   project in your preferred build environment (I use Code) and
   compile a firmware for your application which does nothing. Well,
   not very much.
5. Edit the ```.inc``` files downloaded at (3) to tailor and
   extend the module firmware to the requirements of your
   application.  DO NOT MODIFY myapp.cpp.

