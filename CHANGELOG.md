# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [3.1.1] - 2020-12-14

 * [`changed`] Updated embedded-common to 0.1.0 to improve compatibility when
               using multiple embedded drivers

## [3.1.0] - 2020-10-16

 * [`added`]   Support FW2.x (sleep, wake up, device status register)
 * [`changed`] Update timings according to datasheet
 * [`changed`] Use configuration independent endianness conversions: no need to
               define `SENSIRION_BIG_ENDIAN` anymore.
 * [`changed`] Release zip now only includes needed files from embedded-common

## [3.0.1] - 2019-11-26

 * [`fixed`]   Fix timing with `sps30_get_fan_auto_cleaning_interval` and
               `sps30_get_fan_auto_cleaning_interval_days` which could result in
               read failures.

## [3.0.0] - 2019-11-20

 * [`fixed`]   Improved compatibility with C++ compilers
 * [`changed`] Move the i2c init call out of `probe()` and into the example
 * [`changed`] Split out `default_config.inc` from Makefile to configure paths
               and CFLAGS
 * [`changed`] Only one example with either `hw_i2c` or `sw_i2c` is built,
               depending on `CONFIG_I2C_TYPE`. Defaults to `hw_i2c`.
 * [`added`]   Add `SPS30_MEASUREMENT_DURATION_USEC` to header
 * [`added`]   Add `SPS30_I2C_ADDRESS` to header
 * [`changed`] Fix compilation warnings when compiling the linux user space
               sample implementation with `CONFIG_I2C_TYPE` set to `sw_i2c`
 * [`fixed`]   Fix wrong include path in CFLAGS in default config of SPS30
 * [`fixed`]   Fix compiler error that is caused by variables having stdint
               types as names (e.g. `uint16_t uint16_t = 5`)
 * [`changed`] Update submodule to increase timeout while clock stretching in
               software I2C mode
 * [`fixed`]   Add a delay to the following commands in order to fix an I2C
               write fail that might happen when the sensor is still busy
               processing the command when the next command arrives:
               - sps30_start_measurement
               - sps30_stop_measurement
               - sps30_set_fan_auto_cleaning_interval
               - sps30_set_fan_auto_cleaning_interval_days
               - sps30_start_manual_fan_cleaning
               - sps30_reset
 * [`removed`] Remove the `AUTHORS` file from the driver and the
               `embedded-common` submodule, as it adds more noise than benefit.
               The contributors can be found in the git log.
 * [`fixed`]   Copy correct `CHANGELOG.md` and `LICENSE` files to target
               locations when running the `release` target of the driver's root
               Makefile.

## [2.0.0] - 2019-05-13

 * [`changed`] Use stdint types (e.g. `uint16_t` instead of `u16`)
 * [`added`] `sps_get_driver_version`
 * [`changed`] Update example usage
 * [`fixed`] Arduino example.

## [1.0.0] - 2018-11-14

 * Rename sps30 to sps30-i2c to avoid confusion with sps30-uart

## [0.9.0] - 2018-11-09

 * Initial I2C support for SPS30

[Unreleased]: https://github.com/Sensirion/embedded-sps/compare/3.1.1...master
[3.1.1]: https://github.com/Sensirion/embedded-sps/compare/3.1.0...3.1.1
[3.1.0]: https://github.com/Sensirion/embedded-sps/compare/3.0.1...3.1.0
[3.0.1]: https://github.com/Sensirion/embedded-sps/compare/3.0.0...3.0.1
[3.0.0]: https://github.com/Sensirion/embedded-sps/compare/2.0.0...3.0.0
[2.0.0]: https://github.com/Sensirion/embedded-sps/compare/1.0.0...2.0.0
[1.0.0]: https://github.com/Sensirion/embedded-sps/compare/0.9.0...1.0.0
[0.9.0]: https://github.com/Sensirion/embedded-sps/releases/tag/0.9.0
