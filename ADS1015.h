/**
 * ADS1015.h
 * Created: 08 October 2020
 * Author: Elliott
 *
 * ADS1015 contains four registers that are accessible through the I2C interface 
 * using the Address Pointer register:
 *  - Conversion register
 *  - Configuration register
 *  - Low threshold register
 *  - High threshold register
 */

#ifndef ADS1015_H
#define ADS1015_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* I2C Addresses */
typedef enum {
    ADS1015_I2C_ADDR_GND = 0x48, /* (default) 7-bit unshifted */
    ADS1015_I2C_ADDR_VDD = 0x49,
    ADS1015_I2C_ADDR_SDA = 0x4A,
    ADS1015_I2C_ADDR_SCL = 0x4B,
} ads1015_i2c_address_t;

/* I2C platform specific read/write function pointers */
typedef int32_t (*ads1015_write_ptr)(void *, uint8_t, uint8_t*, uint16_t);
typedef int32_t (*ads1015_read_ptr) (void *, uint8_t, uint8_t*, uint16_t);

typedef struct {
  /* Component mandatory fields */
  ads1015_write_ptr  write_reg;
  ads1015_read_ptr   read_reg;
  /* Customisable optional pointer */
  void *handle;
} ads1015_ctx_t;


/**
 * Address Pointer Registers (Write)
 * Note: Each address/register is 16-bit
 * 0: Read a measurement which is either 1 channel of the diff between 2 channels
 * 1: Read/Write the configuration of the ADS1015
 * 2: Read/Write the low threshold value which alerts when exceeded
 * 2: Read/Write the high threshold value which alerts when exceeded
 */
/*typedef struct
{
    uint8_t value : 2;
    uint16_t reserved : 6;
} ads1015_address_reg_t;*/
typedef enum {
    ADS1015_POINTER_CONVERSION  = 0,
    ADS1015_POINTER_CONFIG      = 1,
    ADS1015_POINTER_LOW_THRESH  = 2,
    ADS1015_POINTER_HIGH_THRESH = 3,
} ads1015_address_pointers_t;


/**
 * Configuration Register (Read/Write)
 */
typedef struct {
    uint8_t mode : 1;       /* MSByte Start */
    uint8_t pga : 3;
    uint8_t mux : 3;
    uint8_t os : 1;         /* MSByte End */
    uint8_t comp_que : 2;   /* LSByte Start */
    uint8_t comp_lat : 1;
    uint8_t comp_pol : 1;
    uint8_t comp_mode : 1;
    uint8_t dr : 3;         /* LSByte End */
} ads1015_config_reg_t;
int32_t ads1015_config_get(ads1015_ctx_t *ctx, ads1015_config_reg_t *config);
int32_t ads1015_config_set(ads1015_ctx_t *ctx, ads1015_config_reg_t config);

#define ADS1015_DEFAULT_CONFIG() \
    {                            \
        .mode = 1,               \
        .pga = 2,                \
        .mux = 0,                \
        .os = 1,                 \
        .comp_que = 3,           \
        .comp_lat = 0,           \
        .comp_pol = 0,           \
        .comp_mode = 0,          \
        .dr = 4,                 \
    }
static inline void ads1015_config_default_get(ads1015_config_reg_t *config)
{
    /* *config = 0x8583; */
    config->mode = 1;
    config->pga = 2;
    config->mux = 0;
    config->os = 1;
    config->comp_que = 3;
    config->comp_lat = 0;
    config->comp_pol = 0;
    config->comp_mode = 0;
    config->dr = 4;
}
static inline uint16_t ads1015_config_to_uint16_t(ads1015_config_reg_t *config)
{
    return config->comp_que
        | config->comp_lat << 2
        | config->comp_pol << 3
        | config->comp_mode << 4
        | config->dr << 5
        | config->mode << 8
        | config->pga << 9
        | config->mux << 12
        | config->os << 15;
}


/* Operational Status (OS) */
typedef enum {
    ADS1015_OS_NO_EFFECT = 0, /* Writing */
    ADS1015_OS_SINGLE = 1,
    ADS1015_OS_NOT_AVAILABLE = 0, /* Reading */
    ADS1015_OS_AVAILABLE = 1,
} ads1015_os_t;
int32_t ads1015_available_get(ads1015_ctx_t *ctx, ads1015_os_t *status);
int32_t ads1015_available_set(ads1015_ctx_t *ctx, ads1015_os_t status);

/* Multiplexer/Channel (Mux) */
typedef enum {
    /* Mux Differential */
    ADS1015_MUX_DIFF_0 = 0, /* P0_N1 (default) */
    ADS1015_MUX_DIFF_1 = 1, /* P0_N3 */
    ADS1015_MUX_DIFF_2 = 2, /* P1_N3 */
    ADS1015_MUX_DIFF_3 = 3, /* P2_N3 */

    /* Mux Single */
    ADS1015_MUX_0 = 4,
    ADS1015_MUX_1 = 5,
    ADS1015_MUX_2 = 6,
    ADS1015_MUX_3 = 7,
} ads1015_channel_t;
int32_t ads1015_channel_get(ads1015_ctx_t *ctx, ads1015_channel_t *channel);
int32_t ads1015_channel_set(ads1015_ctx_t *ctx, ads1015_channel_t channel);

/* Programmable gain amplifier (PGA) */
typedef enum {
    ADS1015_PGA_0 = 0,    /* +/- 6.144v Two Thirds */
    ADS1015_PGA_1 = 1,    /* +/- 4.096v One */
    ADS1015_PGA_2 = 2,    /* +/- 2.048v Two (default) */
    ADS1015_PGA_3 = 3,    /* +/- 1.024v Four */
    ADS1015_PGA_4 = 4,    /* +/- 0.512v Eight */
    ADS1015_PGA_5 = 5,    /* +/- 0.256v Sixteen (5, 6, 7) */
} ads1015_pga_t;
int32_t ads1015_gain_get(ads1015_ctx_t *ctx, ads1015_pga_t *gain);
int32_t ads1015_gain_set(ads1015_ctx_t *ctx, ads1015_pga_t gain);

/* Operating Mode */
typedef enum {
    ADS1015_MODE_CONT = 0,
    ADS1015_MODE_SINGLE = 1, /* (default) */
} ads1015_mode_t;
int32_t ads1015_mode_get(ads1015_ctx_t *ctx, ads1015_mode_t *mode);
int32_t ads1015_mode_set(ads1015_ctx_t *ctx, ads1015_mode_t mode);

/* Data Rate */
typedef enum {
    ADS1015_DR_128HZ = 0,
    ADS1015_DR_250HZ = 1,
    ADS1015_DR_490HZ = 2,
    ADS1015_DR_920HZ = 3,
    ADS1015_DR_1600HZ = 4, /* (default) */
    ADS1015_DR_2400HZ = 5,
    ADS1015_DR_3300HZ = 6, /* (6, 7) */
} ads1015_dr_t;
int32_t ads1015_data_rate_get(ads1015_ctx_t *ctx, ads1015_dr_t *rate);
int32_t ads1015_data_rate_set(ads1015_ctx_t *ctx, ads1015_dr_t rate);

/* Comparator Mode */
typedef enum {
    ADS1015_CMODE_TRAD   = 0, /* Traditional comparator with hysteresis (default) */
    ADS1015_CMODE_WINDOW = 1, /* Window comparator */
} ads1015_comp_mode_t;
int32_t ads1015_comp_mode_get(ads1015_ctx_t *ctx, ads1015_comp_mode_t *mode);
int32_t ads1015_comp_mode_set(ads1015_ctx_t *ctx, ads1015_comp_mode_t mode);

/* Comparator Polarity */
typedef enum {
    ADS1015_CPOL_ACTVLOW = 0, /* ALERT/RDY pin is low when active (default) */
    ADS1015_CPOL_ACTVHI  = 1, /* ALERT/RDY pin is high when active */
} ads1015_comp_pol_t;
int32_t ads1015_comp_pol_get(ads1015_ctx_t *ctx, ads1015_comp_pol_t *pol);
int32_t ads1015_comp_pol_set(ads1015_ctx_t *ctx, ads1015_comp_pol_t pol);

/* Comparator Latching */
typedef enum {
    ADS1015_CLAT_NONLAT = 0, /* Non-latching comparator (default) */
    ADS1015_CLAT_LATCH  = 1, /* Latching comparator */
} ads1015_comp_lat_t;
int32_t ads1015_comp_lat_get(ads1015_ctx_t *ctx, ads1015_comp_lat_t *lat);
int32_t ads1015_comp_lat_set(ads1015_ctx_t *ctx, ads1015_comp_lat_t lat);

/* Comparator Queue */
typedef enum {
    ADS1015_CQUE_1CONV = 0, /* Assert ALERT/RDY after one conversions */
    ADS1015_CQUE_2CONV = 1, /* Assert ALERT/RDY after two conversions */
    ADS1015_CQUE_4CONV = 2, /* Assert ALERT/RDY after four conversions */
    ADS1015_CQUE_NONE  = 3, /* Disable the comparator and put ALERT/RDY in high state (default) */
} ads1015_comp_que_t;
int32_t ads1015_comp_que_get(ads1015_ctx_t *ctx, ads1015_comp_que_t *que);
int32_t ads1015_comp_que_set(ads1015_ctx_t *ctx, ads1015_comp_que_t que);


/**
 * Conversion Register (Read)
 */
int32_t ads1015_measurement_get(ads1015_ctx_t *ctx, ads1015_config_reg_t config, uint16_t *result);


/**
 * Threshold Registers (Read/Write)
 * Note: These registers must be updated/set if the gain (PGA) is changed.
 * Low Default: 0x8000
 * High Default: 0x7fff
 */
int32_t ads1015_low_thresh_get(ads1015_ctx_t *ctx, uint16_t *value);
int32_t ads1015_low_thresh_set(ads1015_ctx_t *ctx, uint16_t value);
int32_t ads1015_high_thresh_get(ads1015_ctx_t *ctx, uint16_t *value);
int32_t ads1015_high_thresh_set(ads1015_ctx_t *ctx, uint16_t value);


#ifdef __cplusplus
}
#endif

#endif /* ADS1015_H */
