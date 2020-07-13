sps_driver_dir ?= ..
sensirion_common_dir ?= ${sps_driver_dir}/embedded-common
sps_common_dir ?= ${sps_driver_dir}/sps-common
sps30_i2c_dir ?= ${sps_driver_dir}/sps30-i2c
CONFIG_I2C_TYPE ?= hw_i2c

sw_i2c_impl_src ?= ${sensirion_common_dir}/sw_i2c/sensirion_sw_i2c_implementation.c
hw_i2c_impl_src ?= ${sensirion_common_dir}/hw_i2c/sensirion_hw_i2c_implementation.c

CFLAGS ?= -Os -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC
ifdef CI
	CFLAGS += -Werror
endif
CFLAGS += -I${sensirion_common_dir} -I${sps_common_dir} -I${sps30_i2c_dir} \
          -I${sensirion_common_dir}/${CONFIG_I2C_TYPE}

sensirion_common_sources = ${sensirion_common_dir}/sensirion_arch_config.h \
                           ${sensirion_common_dir}/sensirion_i2c.h \
                           ${sensirion_common_dir}/sensirion_common.h \
                           ${sensirion_common_dir}/sensirion_common.c

sps_common_sources = ${sps_common_dir}/sps_git_version.h \
                     ${sps_common_dir}/sps_git_version.c

sps30_i2c_sources = ${sensirion_common_sources} ${sps_common_sources} \
                    ${sps30_i2c_dir}/sps30.h ${sps30_i2c_dir}/sps30.c

hw_i2c_sources = ${hw_i2c_impl_src}
sw_i2c_sources = ${sensirion_common_dir}/sw_i2c/sensirion_sw_i2c_gpio.h \
                 ${sensirion_common_dir}/sw_i2c/sensirion_sw_i2c.c \
                 ${sw_i2c_impl_src}
