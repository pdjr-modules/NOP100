# NOP100-SIM

This firmware extension for
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
supports one or two
[MikroE-5981 Digi Isolator 2 Click]()
MikroBus expansion cards providing an NMEA interface to a maximum of
sixteen external switch input channels.

Switch input channels are consolidated into a single NMEA switch bank
whose status is reported by broadcast of a PGN 127501 Binary Switch
Status message once every two seconds or immediately when a state
change is detected on any input channel.

## Hardware requirement

* 1 x NOP100 motherboard;
* A maximum of two [MikroE-5981 Digi Isolator 2 Click]() expansion cards.

## Build

Building the firmware requires that both
[firmware-factory]()
and
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
are installed locally in the locations pointed to by ```${FF}``` and
```${NOP100}```.

```
$> cd "${FF}/sketch"
$> ln -s "${NOP100}/firmware" src
$> pushd src ; ./link-module NOP100-SIM ; popd
$> pio run
```
