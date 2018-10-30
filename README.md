# embedded-sps
This repository contains the embedded driver sources for Sensirion's
SPS product line.

## Clone this repository
```
git clone --recursive https://github.com/Sensirion/embedded-sps.git
```

## Repository content
* `embedded-common` submodule repository for common HAL
* `sps30` SPS30 driver


## Hardware setup
* Make sure that the SPS30's Pin 4 ("Interface select") is connected to GND,
  otherwise the sensor works in UART and instead of i2c mode.
* The i2c wires need appropriate pull-up resistors if they're not included or
  enabled in your microprocessor.

## Collecting resources
```
make release
```
This will create a release folder with the necessary driver files in it,
including a Makefile. That way, you have just ONE folder with all the sources
ready to build your driver for your platform.

## Files to adjust (from embedded-common)
You only need to touch the following files:

* `sensirion_arch_config.h` architecture specifics, you need to specify the
  integer sizes

and depending on your i2c implementation one of the following:

* `sensirion_hw_i2c_implementation.c` (functions for hardware i2c
communication if your platform supports that)
* `sensirion_sw_i2c_implementation.c` (functions for software i2c
communication via GPIOs)


## Building the driver
1. Step into your desired directory (e.g.: `release/sps30`)
2. Adjust sensirion\_arch\_config.h if the `<stdint.h>` header is not available
3. Implement necessary functions in `*_implementation.c`
4. make


---

Please check the [embedded-common](https://github.com/Sensirion/embedded-common)
repository for further information and sample implementations.

---
