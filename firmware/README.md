# NOP100/firmware

```NOP100.cpp``` is a complete, runnable, firmware for NOP100 hardware
with the caveat that the firmware has no real-world application.

If you connect a NOP100 vanilla device to your network it will appear
as a device with Class Code 10 (System Tools) and Function Code 130
(Diagnostic). You can interact with a NAOP100 to the extent of
setting and displaying the module instance number. That's it.

To build a module that performs some real-world task you will need
some supporting hardware
[see here](../hardware/README.md)
and an elaborated NOP100 firmware that exploits it.

The housekeeping and services implemented in the core NOP100 firmware
must be preserved and in an effort to encourage this firmmware code is
implemented so that the application specific functionality required to
implement a particular module is abstracted out into a small
collection of include files which allow even quite complex behaviours
to be built with a minimum of code.

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
