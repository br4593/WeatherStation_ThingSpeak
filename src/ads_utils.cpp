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
    errorInfo.ads1115 = true;
<<<<<<< HEAD
  }

  else
  {
    Serial.println("ADS1115 initialized.");
    errorInfo.ads1115 = false;
=======
>>>>>>> c27b9fdf6d442b10c7bd7b470984b7a59e7c1c98
  }
}
