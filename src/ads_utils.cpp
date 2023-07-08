#include "ads_utils.h"

Adafruit_ADS1115 ads;

/**
 * Set up the ADS1115 ADC module.
 * This function initializes the ADS1115 module and checks for its presence.
 * If the module fails to initialize, it sets the ads1115_error flag to true.
 */
void setupADS1115() {
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    ads1115_error = true;
  }
}
