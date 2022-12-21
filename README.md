# NOP100

NOP100 provides a hardware design and firmware for a generic, abstract,
NMEA2000 module.

## NOP100 hardware

A microcontroller-based hardware design is elaborated with power
supply, CAN interface, LED status display and configuration support,
leaving an array of unused GPIO that can be used to extend this core
hardware into a real-world application.

The core schematic design translates into a PCB layout that can be
easily expanded to implement most varieties of a working NMEA module.

with essentially no real-world functionality.can be easily extended to supportthe requirements of a real, functional,
NMEA 2000 module  and abstracts most of the housekeeping functions necessary to build a working
NMEA 2000 module

The functionality required to implement a particular module
application is abstracted out into a small collection of include
files which allow even quite complex behaviours to be built with
a minimum of code.

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

