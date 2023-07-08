#ifndef BME_UTILS_H
#define BME_UTILS_H

#include "main_utils.h"
#include <Adafruit_BME280.h>

extern Adafruit_BME280 bme;

/**
 * @brief Set up the BME280 temperature, pressure, and humidity sensor.
 * 
 * This function initializes the BME280 sensor module with the specified I2C address.
 * 
 * @note Make sure to call this function before using the BME280 for measurements.
 */
void setupBME280();

/**
 * @brief Read the atmospheric pressure from the BME280 sensor.
 * 
 * This function reads the atmospheric pressure from the BME280 sensor and applies
 * altitude correction to obtain the corrected pressure at sea level.
 * 
 * @return The atmospheric pressure in hPa (hectopascals).
 */
float readPressure();

#endif
