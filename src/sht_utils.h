#ifndef SHT_UTILS_H
#define SHT_UTILS_H

#include "main_utils.h"
#include <Adafruit_SHT31.h>

extern Adafruit_SHT31 sht;

/**
 * @brief Set up the SHT31 temperature and humidity sensor.
 *
 * This function initializes the SHT31 sensor module.
 *
 * @note Make sure to call this function before using the SHT31 for measurements.
 */
void setupSHT31();

/**
 * @brief Read the temperature from the SHT31 sensor.
 *
 * This function reads the temperature from the SHT31 sensor and returns the measured
 * value in degrees Celsius.
 *
 * @return The measured temperature in degrees Celsius.
 */
float readTemperature();

/**
 * @brief Read the humidity from the SHT31 sensor.
 *
 * This function reads the humidity from the SHT31 sensor and returns the measured
 * value in percentage.
 *
 * @return The measured relative humidity in percentage.
 */
float readHumidity();

#endif
