## This file controls the custom user build settings.

## Choose either of hw i2c or sw i2c depending on whether you have a dedicated
## i2c controller (hw) or are using bit-banging on GPIOs (sw)
# CONFIG_I2C_TYPE = hw

## For hw i2c, configure the i2c HAL implementation to use.
## Use one of the available sample-implementations or implement your own using
## the stub.
# hw_i2c_impl_src = ${sensirion_common_dir}/i2c/hw/sensirion_hw_i2c_implementation.c

## For sw i2c, configure the GPIO implementation.
# sw_i2c_impl_src = ${sensirion_common_dir}/i2c/sw/sensirion_sw_i2c_implementation.c

##
## The items below are listed as documentation but may not need customization
##

## The build paths can also be changed here if needed
# sps_driver_dir = ..
# sensirion_common_dir = ${sps_driver_dir}/embedded-common/common
# sensirion_i2c_dir = ${sps_driver_dir}/embedded-common/i2c
# sps_common_dir = ${sps_driver_dir}/sps-common
# sps30_i2c_dir = ${sps_driver_dir}/sps30-i2c

## If you need different CFLAGS, those can be customized as well
# CFLAGS = -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC
