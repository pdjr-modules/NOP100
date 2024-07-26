# NOP100-SIM

This project provides a firmware extension for
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
which implements a 16-channel NMEA 2000 switch input module.

## Hardware requirement

* 1 x NOP100 motherboard;
* 2 x [MikroE-5981 Digi Isolator 2 Click]() expansion cards.

## Build

Building the firmware requires that both
[firmware-factory]()
and
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
are installed locally in the locations pointed to by ```${FF}`` and
```${NOP100}```.

```
$> cd "${FF}/sketch"
$> ln -s "${NOP100}/firmware" src
$> pushd src ; ./link-module NOP100-SIM ; popd
$> pio run
```

## NMEA interface

**NOP100-SIM** broadcasts a PGN 127501 Binary Switch Status message
once every two seconds or immediately when a state change is detected
on a switch input on any of the connected MikroE 5891 modules.

