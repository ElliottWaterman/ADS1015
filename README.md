# ADS1015

Device agnostic library for the ADS1015 (4 channel, 12-bit ADC) written in C (compatible with C++ and Arduino).

## Configuration
There is only _1 configuration register_ for all _4 channels_, this means each device connected to an input channel _must set the config_ before taking a measurement (the current config could be set up for another device/channel that just used the ADS1015).

## Taking a measurement
To take a measurement, call the function `ads1015_measurement_get` with parameters:
- `ads1015_ctx_t`
  - Holds the platform specific I2C functions and customisable pointer.
- `ads1015_config_reg_t`
  - Config values specific to the connected device & channel (i.e. channel, gain, mode, data rate, etc).
  - The _operational status (OS)_ is set to 1 in this function which the ADS1015 needs to start a measurement.
- `uint16_t measurement`
  - To recieve a 12-bit measurement once the conversion has completed.

## How to implement
- Include the header file: `#include "ADS1015.h"`

- Create an instance of `ads1015_ctx_t` and set the _mandatory_ `read_reg` and `write_reg` fields to the platform specific I2C read and write _functions_.

- Create an instance of `ads1015_config_reg_t` and set the values to work with a specific device on a channel.

- Call the function `ads1015_measurement_get` as described under heading [Taking a measurement](#taking-a-measurement).

### Arduino

```
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    Wire.beginTransmission(i2cAddress);
    Wire.write(reg);
    Wire.beginTransmission(false);

    Wire.requestFrom(i2cAddress, len);
    for (uint16_t x = 0; x < len; x++)
    {
        bufp[x] = Wire.read();
    }

    return 0;
}

int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    Wire.beginTransmission(i2cAddress);
    Wire.write(reg);
    for (uint16_t x = 0; x < len; x++)
    {
        Wire.write(bufp[x]);
    }

    return Wire.endTransmission(); // Will return 0 upon success
}


...


ads1015_ctx_t dev_ctx;
dev_ctx.write_reg = platform_write;
dev_ctx.read_reg = platform_read;
//dev_ctx.handle = (void*) this; // Used in a wrapper class


...


ads1015_config_reg_t config = ADS1015_DEFAULT_CONFIG();
// customise default config above with device specific settings below
config.mux = ADS1015_MUX_0; // Input channel: 0
config.pga = ADS1015_PGA_3; // Gain: +/- 1.024V
// etc...


...


uint16_t measurement;
int32_t error_response = ads1015_measurement_get(&dev_ctx, config, &measurement);
if (error_response == 0) {
    // Success, use measurement here
}
else {
    // Failure, handle error here
}

```

### C / C++
Create a single wrapper class
- If each device connected to the ADS1015 input channels are the same.
- Hold a single config object (`ads1015_config_reg_t`) and change the settings if needed.

Create multiple different wrapper classes
- If each device connected to an ADS1015 input channel is completely different.
- Hold a single config object (`ads1015_config_reg_t`) set to a specific channel.
- When getting a measurement the config is set before the ADS1015 performs a conversion and returns a result.
  - This means that different devices will get correct measurements, by setting the config beforehand.
