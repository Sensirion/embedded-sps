sensirion_common_dir := ../embedded-common
sps_common_dir := .
sw_i2c_dir := ${sensirion_common_dir}/sw_i2c
hw_i2c_dir := ${sensirion_common_dir}/hw_i2c

CFLAGS := -Os -Wall -Wstrict-aliasing=1 -I. -I${sensirion_common_dir}

sensirion_common_objects := sensirion_common.o
sps_common_objects := sps_git_version.o
sps_binaries := sps30_example_usage_sw_i2c sps30_example_usage_hw_i2c

sw_objects := sensirion_sw_i2c.o sensirion_sw_i2c_implementation.o
hw_objects := sensirion_hw_i2c_implementation.o
all_objects := ${sensirion_common_objects} ${sps_common_objects} ${hw_objects} ${sw_objects} sps30.o

.PHONY: all

all: ${sps_binaries}

sps_git_version.o: ${sps_common_dir}/sps_git_version.c
	$(CC) $(CFLAGS) -c -o $@ $^
sensirion_common.o: ${sensirion_common_dir}/sensirion_common.c
	$(CC) $(CFLAGS) -c -o $@ $^
sensirion_sw_i2c_implementation.o: ${sw_i2c_dir}/sensirion_sw_i2c_implementation.c
	$(CC) -I${sw_i2c_dir} $(CFLAGS) -c -o $@ $^
sensirion_hw_i2c_implementation.o: ${hw_i2c_dir}/sensirion_hw_i2c_implementation.c
	$(CC) $(CFLAGS) -c -o $@ $^

sensirion_sw_i2c.o: ${sw_i2c_dir}/sensirion_sw_i2c.c
	$(CC) -I${sw_i2c_dir} $(CFLAGS) -c -o $@ $^

sps30.o: ${sensirion_common_dir}/sensirion_arch_config.h ${sensirion_common_dir}/sensirion_i2c.h ${sps_common_dir}/sps_git_version.c sps30.h sps30.c

sps30_example_usage_sw_i2c: ${sensirion_common_objects} ${sps_common_objects} ${sw_objects} sps30.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS) sps30_example_usage.c

sps30_example_usage_hw_i2c: ${sensirion_common_objects} ${sps_common_objects} ${hw_objects} sps30.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS) sps30_example_usage.c


clean:
	$(RM) ${all_objects} ${sps_binaries}
