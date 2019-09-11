# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

 * [`fixed`]  Improved compatibility with C++ compilers
 * [`changed`]  Move the i2c init call out of `probe()` and into the example
 * [`changed`]  Split out `default_config.inc` from Makefile to configure paths
                and CFLAGS
 * [`changed`]  Only one example with either `hw_i2c` or `sw_i2c` is built,
                depending on `CONFIG_I2C_TYPE`. Defaults to `hw_i2c`.
 * [`added`]  Add `SPS30_MEASUREMENT_DURATION_USEC` to header
 * [`added`]  Add `SPS30_I2C_ADDRESS` to header

## [2.0.0] - 2019-05-13

 * [`changed`] Use stdint types (e.g. `uint16_t` instead of `u16`)
 * [`added`] `sps_get_driver_version`
 * [`changed`] Update example usage
 * [`fixed`] Arduino example.

## [1.0.0] - 2018-11-14

 * Rename sps30 to sps30-i2c to avoid confusion with sps30-uart

## [0.9.0] - 2018-11-09

 * Initial I2C support for SPS30

[Unreleased]: https://github.com/Sensirion/embedded-sps/compare/2.0.0...master
[2.0.0]: https://github.com/Sensirion/embedded-sps/compare/1.0.0...2.0.0
[1.0.0]: https://github.com/Sensirion/embedded-sps/compare/0.9.0...1.0.0
[0.9.0]: https://github.com/Sensirion/embedded-sps/releases/tag/0.9.0
