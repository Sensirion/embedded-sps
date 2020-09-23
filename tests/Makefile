include ../embedded-common/test-config/base_config.inc
sps_driver_dir := ${driver_dir}/embedded-sps
include ${sps_driver_dir}/sps30-i2c/default_config.inc

sps30_test_binaries := sps30-test-hw_i2c sps30-test-sw_i2c

.PHONY: all clean prepare test

all: clean prepare test

prepare:
	cd ${sps_driver_dir} && $(MAKE) prepare

sps30-test-hw_i2c: sps30-test.cpp ${sps30_i2c_sources} ${hw_i2c_sources} ${sensirion_test_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sps30-test-sw_i2c: CONFIG_I2C_TYPE := sw_i2c
sps30-test-sw_i2c: sps30-test.cpp ${sps30_i2c_sources} ${sw_i2c_sources} ${sensirion_test_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ${sps30_test_binaries}

test: prepare ${sps30_test_binaries}
	set -ex; for test in ${sps30_test_binaries}; do echo $${test}; ./$${test}; echo; done;
