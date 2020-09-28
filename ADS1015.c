#include "ADS1015.h"

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


/* Conversion Register (Measurement) */
int32_t ads1015_measurement_get(ads1015_ctx_t *ctx, ads1015_config_reg_t config, ads1015_measurement_t *result)
{
    int32_t ret;

    /* Start a single measurement conversion */
    config.os = ADS1015_OS_SINGLE;

    ret = ads1015_config_set(ctx, config);
    if (ret == 0) {
        /* Block until measurement conversion has completed */
        ads1015_os_t ready = ADS1015_OS_NOT_AVAILABLE;
        do {
            ads1015_available_get(ctx, &ready);
        } while (ready != ADS1015_OS_AVAILABLE);

        /* Read new measurement */
        ret = ads1015_read_reg(ctx, ADS1015_POINTER_CONVERSION, (uint8_t*) &result, 2);
    }
    return ret;
}


/* Configuration Register */
int32_t ads1015_config_get(ads1015_ctx_t *ctx, ads1015_config_reg_t *config)
{
    int32_t ret;
    ret = ads1015_read_reg(ctx, ADS1015_POINTER_CONFIG, (uint8_t*) config, 2);
    return ret;
}
int32_t ads1015_config_set(ads1015_ctx_t *ctx, ads1015_config_reg_t config)
{
    int32_t ret;
    ret = ads1015_write_reg(ctx, ADS1015_POINTER_CONFIG, (uint8_t*) &config, 2);
    return ret;
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


/* Threshold Registers */
int32_t ads1015_low_thresh_get(ads1015_ctx_t *ctx, ads1015_threshold_t *value)
{
    return ads1015_read_reg(ctx, ADS1015_POINTER_LOW_THRESH, (uint8_t*) value, 2);
}
int32_t ads1015_low_thresh_set(ads1015_ctx_t *ctx, ads1015_threshold_t value)
{
    return ads1015_write_reg(ctx, ADS1015_POINTER_LOW_THRESH, (uint8_t*) &value, 2);
}
int32_t ads1015_high_thresh_get(ads1015_ctx_t *ctx, ads1015_threshold_t *value)
{
    return ads1015_read_reg(ctx, ADS1015_POINTER_HIGH_THRESH, (uint8_t*) value, 2);
}
int32_t ads1015_high_thresh_set(ads1015_ctx_t *ctx, ads1015_threshold_t value)
{
    return ads1015_write_reg(ctx, ADS1015_POINTER_HIGH_THRESH, (uint8_t*) &value, 2);
}