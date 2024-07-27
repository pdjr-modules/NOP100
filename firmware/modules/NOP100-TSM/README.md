# NOP100-TSM

This sub-project provides a firmware extension for
[NOP100](https://www.github.com/pdjr-n2k/NOP100)
which implements an 8-channel NMEA 2000 temperature sensor module.

The module supports up to eight
[DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf)
digital thermometers.

A **NOP100-TSM** module broadcasts a
[PGN 130316 Temperature, Extended Range](https://www.nmea.org/Assets/nmea%202000%20pgn%20130316%20corrigenda%20nmd%20version%202.100%20feb%202015.pdf)
message once every five seconds.

## Hardware requirement

* 1 x NOP100 motherboard;
* 1 x [MikroE 1892 I2C 1-Wire Click](https://www.mikroe.com/i2c-1-wire-click) expansion card;
* Up to 8 x DS18B20 temperature sensors.

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
$> pushd src ; ./link-module NOP100-TSM ; popd
$> pio run
```
