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
software interface which supports the entry of data a byte at a time by
entry of a storage address followed by the value to be stored at that
address.

implements a simple configuration mechanism based on the model
saving byte values setting of the NOP100 DIL switch is accessed by by a single
function which can be overri 

The fundamental function (which you can override if you wish) is
prgButtonHandler(boolean) which is called each time the PRG button
state changes. A true argument indicates that the button has been
released; a false argument that it has been pressed.

The NOP100 module supports a simple mechanism for saving data entered
through the NOP100 DIL switch to the Teensy microcontroller's
persistent EEPROM storage.

The fundamental function (which you can override if you wish) is
prgButtonHandler(boolean) which is called each time the PRG button
state changes. A true argument indicates that the button has been
released; a false argument that it has been pressed.

The default config with a single boolean
argument: false sa

The PRG button supports a short-press (less than 1s between press and
release) and a long press (more than 1s between press and release).

Module configuration is handled by the function
```configureModuleSettingMaybe(int)``` which is called in the following
ways.

configureModuleSettingMaybe(0xffff) is called from loop() and is
used to mange the


function with the value on the module

Each time the PRG button button is released a call is made to the
configureModuleSettingMaybe() function with the value on the module
DIL switch passed in the low 8-bits of the function's integer
argument.
Bit 8 of the argument is set to 0 if the call resulted from a short
press of the PRG button and to 1 if it resulted from a long press.

An additional call to configureModuleSettingsMaybe() is made from
loop wih an argument of zero.  

You can override the default configuration function by redefining the

The module's NMEA instance number can be set using the following protocol:

1. Enter the required module instance number on the ```ADDR/VALUE``` DIL
   switch.
   
2. Press and release the ```PRG``` button. The new instance number will
   enter use immediately and be saved to EEPROM. The module's transmit
   LED will flash three times to confirm the update.
   
General purpose application settings consist of an 8-bit address and an
8-bit value. A setting is entered using the following protocol:

1. Enter the setting address on the ```ADDR/VALUE``` DIL switch.

2. Press and hold the ```PRG``` button for over 1 second then release.
   The transmit LED will begin flashing regularly.
   
3. Enter the setting value on the ```ADDR/VALUE``` DIL switch.

4. Press and release the ```PRG``` button.

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

