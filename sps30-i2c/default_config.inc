sps_driver_dir ?= ..
sensirion_common_dir ?= ${sps_driver_dir}/embedded-common/common
sensirion_i2c_dir ?= ${sps_driver_dir}/embedded-common/i2c
sps_common_dir ?= ${sps_driver_dir}/sps-common
sps30_i2c_dir ?= ${sps_driver_dir}/sps30-i2c
CONFIG_I2C_TYPE ?= hw

sw_i2c_impl_src ?= ${sensirion_i2c_dir}/sw/sensirion_sw_i2c_implementation.c
hw_i2c_impl_src ?= ${sensirion_i2c_dir}/hw/sensirion_hw_i2c_implementation.c

CFLAGS ?= -Os -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC
ifdef CI
	CFLAGS += -Werror
endif
CFLAGS += -I${sensirion_common_dir} -I${sps_common_dir} -I${sps30_i2c_dir} \
          -I${sensirion_i2c_dir}/${CONFIG_I2C_TYPE} -I${sensirion_i2c_dir}

sensirion_common_sources = ${sensirion_common_dir}/sensirion_config.h \
                           ${sensirion_i2c_dir}/sensirion_i2c.h \
                           ${sensirion_i2c_dir}/sensirion_i2c.c \
                           ${sensirion_i2c_dir}/sensirion_i2c_hal.h \
                           ${sensirion_common_dir}/sensirion_common.h \
                           ${sensirion_common_dir}/sensirion_common.c

sps_common_sources = ${sps_common_dir}/sps_git_version.h \
                     ${sps_common_dir}/sps_git_version.c

sps30_i2c_sources = ${sensirion_common_sources} ${sps_common_sources} \
                    ${sps30_i2c_dir}/sps30.h ${sps30_i2c_dir}/sps30.c

hw_i2c_sources = ${hw_i2c_impl_src}
sw_i2c_sources = ${sensirion_i2c_dir}/sw/sensirion_sw_i2c_gpio.h \
                 ${sensirion_i2c_dir}/sw/sensirion_sw_i2c.c \
                 ${sw_i2c_impl_src}
