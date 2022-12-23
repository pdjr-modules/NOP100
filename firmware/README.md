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

NOP100 models configuration data as an array of bytes and provides a
user interface which allows configuration data to be entered one byte
at-a-time by provision of a storage address followed by the value to be
stored at that address.

| Address | Saved value |
| ---:    | :---        |
| 0       | CAN source address. Not available for application use. |
| 1       | Module instance number. Default storage location used if a value is entered without a preceeding address. |
| 2..1080 | Available for application use. |

The basic programming protocol is enter a value on the PCB's DIL switch
and press and release PRG to confirm.
A short press of PRG signifies entry of a value; a long press of PRG
signifies entry of an address and the expectation is that this will
immediately be followed by entry of a value which should be stored at
the pre-set address.

If a value is entered without a preceeding address then the entered
value is stored at address 1 becoming the module's new instance number.

Storage locations with an address greater than or equal to two are
intended for application configuration and the required protocol is
entry of an address followed by entry of a value to be stored at that
address.

Once an address is accepted, the transmit LED will flash rapidly
indicating that the system is waiting for entry of a data value.
If a value is not entered within one minute the protocol will
self-cancel and must be re-started.

The fundamental function handling this process is ```prgButtonHandler(boolean)```
which is called each time the PRG button state changes. A true argument
indicates that the button has been released; a false argument that it
has been pressed.

You can override the built-in bhaviour and implement your own by
overriding the handler in ```module-declarations.inc```.
The following example disables configuration entirely:
```
#define PRG_BUTTON_HANDLER
void prgButtonHandler(bool state) {
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

