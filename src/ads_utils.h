#ifndef ADS_UTILS_H
#define ADS_UTILS_H

#include "main_utils.h"
#include <Adafruit_ADS1X15.h>


extern Adafruit_ADS1115 ads;

/**
 * @brief Set up the ADS1115 analog-to-digital converter.
 * 
 * This function initializes the ADS1115 ADC module and configures it for
 * single-ended measurements.
 * 
 * @note Make sure to call this function before using the ADS1115 for measurements.
 */
void setupADS1115();

#endif
