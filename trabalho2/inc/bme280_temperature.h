#ifndef _BME280_TEMPERATURE_DRIVER_
#define _BME280_TEMPERATURE_DRIVER_

#include "bme280.h"

#define I2C_DIR_PATH "/dev/i2c-1" // rasp 47
#define BME280_RESPONSE_SUCCESS 0
#define BME280_RESPONSE_FAIL -1

int init_bme280();
void bme280_driver_close(void);
void user_delay_us(uint32_t period, void *intf_ptr);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int32_t get_temperature(double *temperature);

#endif