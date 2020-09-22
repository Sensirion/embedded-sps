#include "sensirion_test_setup.h"
#include "sps30.h"

// Measurement ranges according to datasheet
#define SPS30_MIN_MC 0
#define SPS30_MAX_MC 1000
#define SPS30_MIN_NC 0
#define SPS30_MAX_NC 3000

#define SECONDS_PER_DAY 86400u  // 24h/d * 60m/h * 60s/m

// Less or equal (<=) for float arithmetics with small error (1e-5)
#define LEQ(l, u) ((l) - (u) < 1e-5)

static void sps30_delayed_reset() {
    int16_t ret = sps30_reset();
    CHECK_ZERO_TEXT(ret, "sps30_reset in test reset");
    sensirion_sleep_usec(SPS30_RESET_DELAY_USEC);
}

static void sps30_test(uint8_t set_interval_days) {
    uint8_t get_interval_days;
    int16_t ret;
    uint16_t data_ready;
    uint32_t get_interval_seconds;
    uint32_t set_interval_seconds;
    struct sps30_measurement m;

    // Start measurement
    ret = sps30_start_measurement();
    CHECK_ZERO_TEXT(ret, "sps30_start_measurement");

    // Check if there is old data available
    ret = sps30_read_data_ready(&data_ready);
    CHECK_ZERO_TEXT(ret, "sps30_read_data_ready for old data");

    if (data_ready) {
        // Get old data and discard it
        ret = sps30_read_measurement(&m);
        CHECK_ZERO_TEXT(ret, "sps30_read_measurement for old data");
    }

    // Sleep while waiting for new measurement
    sensirion_sleep_usec(SPS30_MEASUREMENT_DURATION_USEC - 1e5);

    // Wait for data to be ready
    do {
        sensirion_sleep_usec(1e5);  // Sleep 100ms
        ret = sps30_read_data_ready(&data_ready);
        CHECK_ZERO_TEXT(ret, "sps30_read_data_ready while polling");
    } while (!data_ready);

    // Read measurement
    ret = sps30_read_measurement(&m);
    CHECK_ZERO_TEXT(ret, "sps30_read_measurement");
    printf("measured values:\n"
           "\t%0.2f pm1.0\n"
           "\t%0.2f pm2.5\n"
           "\t%0.2f pm4.0\n"
           "\t%0.2f pm10.0\n"
           "\t%0.2f nc0.5\n"
           "\t%0.2f nc1.0\n"
           "\t%0.2f nc2.5\n"
           "\t%0.2f nc4.5\n"
           "\t%0.2f nc10.0\n"
           "\t%0.2f typical particle size\n\n",
           m.mc_1p0, m.mc_2p5, m.mc_4p0, m.mc_10p0, m.nc_0p5, m.nc_1p0,
           m.nc_2p5, m.nc_4p0, m.nc_10p0, m.typical_particle_size);

    // Check if mass concentration is rising monotonously
    CHECK_TRUE_TEXT(LEQ(SPS30_MIN_MC, m.mc_1p0) && LEQ(m.mc_1p0, m.mc_2p5) &&
                        LEQ(m.mc_2p5, m.mc_4p0) && LEQ(m.mc_4p0, m.mc_10p0) &&
                        LEQ(m.mc_10p0, SPS30_MAX_MC),
                    "Mass concentration not rising monotonously");

    // Check if number concentration is rising monotonously
    CHECK_TRUE_TEXT(LEQ(SPS30_MIN_NC, m.nc_0p5) && LEQ(m.nc_0p5, m.nc_1p0) &&
                        LEQ(m.nc_1p0, m.nc_2p5) && LEQ(m.nc_2p5, m.nc_4p0) &&
                        LEQ(m.nc_4p0, m.nc_10p0) &&
                        LEQ(m.nc_10p0, SPS30_MAX_NC),
                    "Number concentration not rising monotonously");

    // Set auto cleaning interval in days, reset and get it again
    ret = sps30_set_fan_auto_cleaning_interval_days(set_interval_days);
    CHECK_ZERO_TEXT(ret, "sps30_set_fan_auto_cleaning_interval_days");
    sps30_delayed_reset();
    ret = sps30_get_fan_auto_cleaning_interval_days(&get_interval_days);
    CHECK_ZERO_TEXT(ret, "sps30_get_fan_auto_cleaning_interval_days");
    CHECK_EQUAL_TEXT(set_interval_days, get_interval_days,
                     "Fan auto cleaning interval days do not match");

    // Set auto cleaning interval in days, reset and set it again
    set_interval_seconds = set_interval_days * SECONDS_PER_DAY;
    ret = sps30_set_fan_auto_cleaning_interval(set_interval_seconds);
    CHECK_ZERO_TEXT(ret, "sps30_set_fan_auto_cleaning_interval");
    sps30_delayed_reset();
    ret = sps30_get_fan_auto_cleaning_interval(&get_interval_seconds);
    CHECK_ZERO_TEXT(ret, "sps30_get_fan_auto_cleaning_interval");
    CHECK_EQUAL_TEXT(set_interval_seconds, get_interval_seconds,
                     "Fan auto cleaning interval seconds do not match");

    // Start manual cleaning event
    ret = sps30_start_manual_fan_cleaning();
    CHECK_ZERO_TEXT(ret, "sps30_start_manual_fan_cleaning");

    // Stop measurement
    ret = sps30_stop_measurement();
    CHECK_ZERO_TEXT(ret, "sps30_stop_measurement");
}

static void sps30_test_setup() {
    uint8_t fs_major;
    uint8_t fs_minor;
    int16_t ret;
    char serial[SPS30_MAX_SERIAL_LEN];

    ret = sps30_probe();
    CHECK_ZERO_TEXT(ret, "sps30_probe in test setup");

    sps30_delayed_reset();

    const char* version = sps_get_driver_version();
    printf("sps30_get_driver_version: %s\n", version);

    ret = sps30_get_serial(serial);
    CHECK_ZERO_TEXT(ret, "sps30_get_serial");
    printf("SPS30 serial: %s\n", serial);

    ret = sps30_read_firmware_version(&fs_major, &fs_minor);
    CHECK_ZERO_TEXT(ret, "sps30_read_firmware_version");
    printf("SPS30 Firmware version: %u.%u\n", fs_major, fs_minor);
}

TEST_GROUP (SPSTestGroup) {
    void setup() {
        int16_t ret;
        sensirion_i2c_init();
        ret = sensirion_i2c_mux_set_single_channel(0x72, 3);
        CHECK_EQUAL_TEXT(0, ret, "sensirion_i2c_mux_select_TEXT(0x72, 3)");
        sps30_test_setup();
    }

    void teardown() {
        sps30_delayed_reset();
        sensirion_i2c_release();
    }
};

TEST (SPSTestGroup, SPS30Test_no_cleaning) { sps30_test(0); }

TEST (SPSTestGroup, SPS30Test_daily_cleaning) { sps30_test(1); }

TEST (SPSTestGroup, SPS30Test_weekly_cleaning) { sps30_test(7); }

TEST (SPSTestGroup, SPS30Test_maximum_cleaning) { sps30_test(255); }
