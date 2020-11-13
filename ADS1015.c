/**
 * ADS1015.c
 * Created: 08 October 2020
 * Author: Elliott
 */

#include "ADS1015.h"

#include <string.h>

/*** Local Data ***/

// static ads1015_config_reg_t last_config;

/*** Local Functions ***/

/**
 * @brief  Read generic device register
 * 
 * @param  ctx   read / write interface definitions(ptr)
 * @param  reg   register to read
 * @param  data  data to be read from the register(ptr)
 * @param  len   number of consecutive bytes to read
 * @return       interface status (MANDATORY: return 0 -> no Error)
 */
int32_t ads1015_read_reg(ads1015_ctx_t* ctx, uint8_t reg, uint8_t* data, uint16_t len)
{
    int32_t ret;
    ret = ctx->read_reg(ctx->handle, reg, data, len);
    return ret;
}

/**
 * @brief  Write generic device register
 * 
 * @param  ctx   read / write interface definitions(ptr)
 * @param  reg   register to write
 * @param  data  data to be written to the register(ptr)
 * @param  len   number of consecutive bytes to write
 * @return       interface status (MANDATORY: return 0 -> no Error)
 */
int32_t ads1015_write_reg(ads1015_ctx_t* ctx, uint8_t reg, uint8_t* data, uint16_t len)
{
    int32_t ret;
    ret = ctx->write_reg(ctx->handle, reg, data, len);
    return ret;
}

inline static bool config_equal(const ads1015_config_reg_t *a, const ads1015_config_reg_t *b)
{
    return a->comp_que == b->comp_que
        && a->comp_lat == b->comp_lat
        && a->comp_pol == b->comp_pol
        && a->dr == b->dr
        && a->mode == b->mode
        && a->pga == b->pga
        && a->mux == b->mux
        && a->os == b->os;
}

/*** Global Functions ***/

/* Configuration Register */
int32_t ads1015_config_get(ads1015_ctx_t *ctx, ads1015_config_reg_t *config)
{
    int32_t ret;
    ret = ads1015_read_reg(ctx, ADS1015_POINTER_CONFIG, (uint8_t*) config, 2);
    return ret;
}
int32_t ads1015_config_set(ads1015_ctx_t *ctx, ads1015_config_reg_t config)
{
    int32_t ret = 0;
    ret = ads1015_write_reg(ctx, ADS1015_POINTER_CONFIG, (uint8_t*) &config, 2);
    return ret;

    // bool different_config = !config_equal(&config, &last_config);

    // if (config.mode == ADS1015_MODE_SINGLE || different_config) {
    //     ret = ads1015_write_reg(ctx, ADS1015_POINTER_CONFIG, (uint8_t*) &config, 2);
    // }

    // if (different_config) {
    //     /* Save latest configuration */
    //     memcpy(&last_config, &config, sizeof(ads1015_config_reg_t));
    // }
    // return ret;
}

/* Operational Status (OS) */
int32_t ads1015_available_get(ads1015_ctx_t *ctx, ads1015_os_t *status)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *status = (ads1015_os_t) config.os;
    }
    return ret;
}
int32_t ads1015_available_set(ads1015_ctx_t *ctx, ads1015_os_t status)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.os != (uint8_t) status) {
        config.os = (uint8_t) status;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Multiplexer/Channel (Mux) */
int32_t ads1015_channel_get(ads1015_ctx_t *ctx, ads1015_channel_t *channel)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *channel = (ads1015_pga_t) config.mux;
    }
    return ret;
}
int32_t ads1015_channel_set(ads1015_ctx_t *ctx, ads1015_channel_t channel)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.mux != (uint8_t) channel) {
        config.mux = (uint8_t) channel;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Programmable gain amplifier (PGA) */
int32_t ads1015_gain_get(ads1015_ctx_t *ctx, ads1015_pga_t *gain)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *gain = (ads1015_pga_t) config.pga;
    }
    return ret;
}
int32_t ads1015_gain_set(ads1015_ctx_t *ctx, ads1015_pga_t gain)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.pga != (uint8_t) gain) {
        config.pga = (uint8_t) gain;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Operating Mode */
int32_t ads1015_mode_get(ads1015_ctx_t *ctx, ads1015_mode_t *mode)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *mode = (ads1015_mode_t) config.mode;
    }
    return ret;
}
int32_t ads1015_mode_set(ads1015_ctx_t *ctx, ads1015_mode_t mode)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.mode != (uint8_t) mode) {
        config.mode = (uint8_t) mode;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Data Rate */
int32_t ads1015_data_rate_get(ads1015_ctx_t *ctx, ads1015_dr_t *rate)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *rate = (ads1015_dr_t) config.dr;
    }
    return ret;
}
int32_t ads1015_data_rate_set(ads1015_ctx_t *ctx, ads1015_dr_t rate)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.dr != (uint8_t) rate) {
        config.dr = (uint8_t) rate;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Comparator Mode */
int32_t ads1015_comp_mode_get(ads1015_ctx_t *ctx, ads1015_comp_mode_t *mode)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *mode = (ads1015_dr_t) config.comp_mode;
    }
    return ret;
}
int32_t ads1015_comp_mode_set(ads1015_ctx_t *ctx, ads1015_comp_mode_t mode)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.comp_mode != (uint8_t) mode) {
        config.comp_mode = (uint8_t) mode;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Comparator Polarity */
int32_t ads1015_comp_pol_get(ads1015_ctx_t *ctx, ads1015_comp_pol_t *pol)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *pol = (ads1015_dr_t) config.comp_pol;
    }
    return ret;
}
int32_t ads1015_comp_pol_set(ads1015_ctx_t *ctx, ads1015_comp_pol_t pol)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.comp_pol != (uint8_t) pol) {
        config.comp_pol = (uint8_t) pol;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Comparator Latching */
int32_t ads1015_comp_lat_get(ads1015_ctx_t *ctx, ads1015_comp_lat_t *lat)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *lat = (ads1015_dr_t) config.comp_lat;
    }
    return ret;
}
int32_t ads1015_comp_lat_set(ads1015_ctx_t *ctx, ads1015_comp_lat_t lat)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.comp_lat != (uint8_t) lat) {
        config.comp_lat = (uint8_t) lat;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}

/* Comparator Queue */
int32_t ads1015_comp_que_get(ads1015_ctx_t *ctx, ads1015_comp_que_t *que)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0) {
        *que = (ads1015_dr_t) config.comp_que;
    }
    return ret;
}
int32_t ads1015_comp_que_set(ads1015_ctx_t *ctx, ads1015_comp_que_t que)
{
    int32_t ret;
    ads1015_config_reg_t config;

    ret = ads1015_config_get(ctx, &config);
    if (ret == 0 && config.comp_que != (uint8_t) que) {
        config.comp_que = (uint8_t) que;
        ret = ads1015_config_set(ctx, config);
    }
    return ret;
}


/* Conversion Register (Measurement) */
int32_t ads1015_measurement_get(ads1015_ctx_t *ctx, ads1015_config_reg_t config, uint16_t *result)
{
    int32_t ret;
    uint8_t buffer[2];

    /* Start a single measurement conversion */
    config.os = ADS1015_OS_SINGLE;

    ret = ads1015_config_set(ctx, config);
    if (ret == 0) {
        /* Block until measurement conversion has completed */
        ads1015_os_t ready = ADS1015_OS_NOT_AVAILABLE;
        do {
            // TODO: Check return is 0, otherwise exit loop as I2C communication has failed
            ads1015_available_get(ctx, &ready);
        } while (ready != ADS1015_OS_AVAILABLE);

        /* Read new measurement */
        ret = ads1015_read_reg(ctx, ADS1015_POINTER_CONVERSION, buffer, 2);
        if (ret == 0) {
            // 12-bit measurement keeping these bits -> 0x[FF][F0] into 0x0[FF][F]
            *result = ((uint16_t) buffer[0] << 4) + ((uint16_t) buffer[1] >> 4);
        }
    }
    return ret;
}


/* Threshold Registers */
int32_t ads1015_low_thresh_get(ads1015_ctx_t *ctx, uint16_t *value)
{
    int32_t ret;
    uint8_t buffer[2];

    ret = ads1015_read_reg(ctx, ADS1015_POINTER_LOW_THRESH, buffer, 2);
    if (ret == 0) {
        // 12-bit measurement keeping these bits -> 0x[FF][F0] into 0x0[FF][F]
        *value = ((uint16_t) buffer[0] << 4) + ((uint16_t) buffer[1] >> 4);
    }
    return ret;
}
int32_t ads1015_low_thresh_set(ads1015_ctx_t *ctx, uint16_t value)
{
    uint8_t buffer[2];
    // 12-bit threshold keeping these bits -> 0x0[FF][F] into 0x[FF][F0]
    buffer[0] = (uint8_t)((value >> 4) & 0x00FF);
    buffer[1] = (uint8_t)((value << 4) & 0x00FF);
    return ads1015_write_reg(ctx, ADS1015_POINTER_LOW_THRESH, buffer, 2);
}
int32_t ads1015_high_thresh_get(ads1015_ctx_t *ctx, uint16_t *value)
{
    int32_t ret;
    uint8_t buffer[2];

    ret = ads1015_read_reg(ctx, ADS1015_POINTER_HIGH_THRESH, buffer, 2);
    if (ret == 0) {
        // 12-bit measurement keeping these bits -> 0x[FF][F0] into 0x0[FF][F]
        *value = ((uint16_t) buffer[0] << 4) + ((uint16_t) buffer[1] >> 4);
    }
    return ret;
}
int32_t ads1015_high_thresh_set(ads1015_ctx_t *ctx, uint16_t value)
{
    uint8_t buffer[2];
    // 12-bit threshold keeping these bits -> 0x0[FF][F] into 0x[FF][F0]
    buffer[0] = (uint8_t)((value >> 4) & 0x00FF);
    buffer[1] = (uint8_t)((value << 4) & 0x00FF);
    return ads1015_write_reg(ctx, ADS1015_POINTER_HIGH_THRESH, buffer, 2);
}
