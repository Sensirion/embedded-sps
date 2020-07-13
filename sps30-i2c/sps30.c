/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sps30.h"
#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sps_git_version.h"

#define SPS_CMD_START_MEASUREMENT 0x0010
#define SPS_CMD_START_MEASUREMENT_ARG 0x0300
#define SPS_CMD_STOP_MEASUREMENT 0x0104
#define SPS_CMD_READ_MEASUREMENT 0x0300
#define SPS_CMD_GET_DATA_READY 0x0202
#define SPS_CMD_AUTOCLEAN_INTERVAL 0x8004
#define SPS_CMD_GET_FIRMWARE_VERSION 0xd100
#define SPS_CMD_GET_SERIAL 0xd033
#define SPS_CMD_RESET 0xd304
#define SPS_CMD_START_MANUAL_FAN_CLEANING 0x5607
#define SPS_CMD_DELAY_USEC 10000
#define SPS_WRITE_DELAY_USEC 15000

const char *sps_get_driver_version() {
    return SPS_DRV_VERSION_STR;
}

int16_t sps30_probe() {
    char serial[SPS30_MAX_SERIAL_LEN];

    return sps30_get_serial(serial);
}

int16_t sps30_read_firmware_version(uint8_t *major, uint8_t *minor) {
    uint16_t version;
    int16_t ret;

    ret = sensirion_i2c_read_cmd(SPS30_I2C_ADDRESS,
                                 SPS_CMD_GET_FIRMWARE_VERSION, &version, 1);
    *major = (version & 0xff00) >> 8;
    *minor = (version & 0x00ff);
    return ret;
}

int16_t sps30_get_serial(char *serial) {
    uint16_t i;
    int16_t ret;
    union {
        char serial[SPS30_MAX_SERIAL_LEN];
        uint16_t __enforce_alignment;
    } buffer;

    ret = sensirion_i2c_read_cmd(SPS30_I2C_ADDRESS, SPS_CMD_GET_SERIAL,
                                 (uint16_t *)buffer.serial,
                                 SENSIRION_NUM_WORDS(buffer.serial));
    if (ret != STATUS_OK)
        return ret;

    SENSIRION_WORDS_TO_BYTES(buffer.serial, SENSIRION_NUM_WORDS(buffer.serial));
    for (i = 0; i < SPS30_MAX_SERIAL_LEN; ++i) {
        serial[i] = buffer.serial[i];
        if (serial[i] == '\0')
            return 0;
    }

    return 0;
}

int16_t sps30_start_measurement() {
    const uint16_t arg = SPS_CMD_START_MEASUREMENT_ARG;

    int16_t ret = sensirion_i2c_write_cmd_with_args(
        SPS30_I2C_ADDRESS, SPS_CMD_START_MEASUREMENT, &arg,
        SENSIRION_NUM_WORDS(arg));

    sensirion_sleep_usec(SPS_CMD_DELAY_USEC);

    return ret;
}

int16_t sps30_stop_measurement() {
    int16_t ret =
        sensirion_i2c_write_cmd(SPS30_I2C_ADDRESS, SPS_CMD_STOP_MEASUREMENT);
    sensirion_sleep_usec(SPS_CMD_DELAY_USEC);
    return ret;
}

int16_t sps30_read_data_ready(uint16_t *data_ready) {
    return sensirion_i2c_read_cmd(SPS30_I2C_ADDRESS, SPS_CMD_GET_DATA_READY,
                                  data_ready, SENSIRION_NUM_WORDS(*data_ready));
}

int16_t sps30_read_measurement(struct sps30_measurement *measurement) {
    int16_t error;
    uint8_t data[10][4];

    error =
        sensirion_i2c_write_cmd(SPS30_I2C_ADDRESS, SPS_CMD_READ_MEASUREMENT);
    if (error != NO_ERROR) {
        return error;
    }

    error = sensirion_i2c_read_words_as_bytes(SPS30_I2C_ADDRESS, &data[0][0],
                                              SENSIRION_NUM_WORDS(data));

    if (error != NO_ERROR) {
        return error;
    }

    measurement->mc_1p0 = sensirion_bytes_to_float(data[0]);
    measurement->mc_2p5 = sensirion_bytes_to_float(data[1]);
    measurement->mc_4p0 = sensirion_bytes_to_float(data[2]);
    measurement->mc_10p0 = sensirion_bytes_to_float(data[3]);
    measurement->nc_0p5 = sensirion_bytes_to_float(data[4]);
    measurement->nc_1p0 = sensirion_bytes_to_float(data[5]);
    measurement->nc_2p5 = sensirion_bytes_to_float(data[6]);
    measurement->nc_4p0 = sensirion_bytes_to_float(data[7]);
    measurement->nc_10p0 = sensirion_bytes_to_float(data[8]);
    measurement->typical_particle_size = sensirion_bytes_to_float(data[9]);

    return 0;
}

int16_t sps30_get_fan_auto_cleaning_interval(uint32_t *interval_seconds) {
    uint8_t data[4];
    int16_t error;

    error =
        sensirion_i2c_write_cmd(SPS30_I2C_ADDRESS, SPS_CMD_AUTOCLEAN_INTERVAL);
    if (error != NO_ERROR) {
        return error;
    }

    sensirion_sleep_usec(SPS_CMD_DELAY_USEC);

    error = sensirion_i2c_read_words_as_bytes(SPS30_I2C_ADDRESS, data,
                                              SENSIRION_NUM_WORDS(data));
    if (error != NO_ERROR) {
        return error;
    }

    *interval_seconds = sensirion_bytes_to_uint32_t(data);

    return 0;
}

int16_t sps30_set_fan_auto_cleaning_interval(uint32_t interval_seconds) {
    int16_t ret;
    const uint16_t data[] = {(uint16_t)((interval_seconds & 0xFFFF0000) >> 16),
                             (uint16_t)(interval_seconds & 0x0000FFFF)};

    ret = sensirion_i2c_write_cmd_with_args(SPS30_I2C_ADDRESS,
                                            SPS_CMD_AUTOCLEAN_INTERVAL, data,
                                            SENSIRION_NUM_WORDS(data));
    sensirion_sleep_usec(SPS_WRITE_DELAY_USEC);
    return ret;
}

int16_t sps30_get_fan_auto_cleaning_interval_days(uint8_t *interval_days) {
    int16_t ret;
    uint32_t interval_seconds;

    ret = sps30_get_fan_auto_cleaning_interval(&interval_seconds);
    if (ret < 0)
        return ret;

    *interval_days = interval_seconds / (24 * 60 * 60);
    return ret;
}

int16_t sps30_set_fan_auto_cleaning_interval_days(uint8_t interval_days) {
    return sps30_set_fan_auto_cleaning_interval((uint32_t)interval_days * 24 *
                                                60 * 60);
}

int16_t sps30_start_manual_fan_cleaning() {
    int16_t ret;

    ret = sensirion_i2c_write_cmd(SPS30_I2C_ADDRESS,
                                  SPS_CMD_START_MANUAL_FAN_CLEANING);
    if (ret)
        return ret;

    sensirion_sleep_usec(SPS_CMD_DELAY_USEC);
    return 0;
}

int16_t sps30_reset() {
    return sensirion_i2c_write_cmd(SPS30_I2C_ADDRESS, SPS_CMD_RESET);
}
