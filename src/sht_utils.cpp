#include "sht_utils.h"

Adafruit_SHT31 sht;

const int SHT_ADDRESS = 0x44;

/**
 * Set up the SHT31 sensor module.
 * This function initializes the SHT31 sensor and checks for its presence.
 * If the sensor is not found, it sets the sht31_error flag to true.
 */
void setupSHT31() {
  if (!sht.begin(SHT_ADDRESS)) {
    Serial.println("Couldn't find SHT31 sensor! Please check the wiring.");
    errorInfo.sht31 = true;
  }

  else{
    Serial.println("SHT31 sensor found!");
    errorInfo.sht31 = false;
  }
  
  // Read and print the sensor status for debugging purposes
  uint16_t stat = sht.readStatus();
  Serial.print("SHT31 Status: 0x");
  Serial.println(stat, HEX);
}

/**
 * Read the temperature from the SHT31 sensor.
 *
 * @return The temperature reading in degrees Celsius.
 */
float readTemperature() {
  return sht.readTemperature();
}

/**
 * Read the humidity from the SHT31 sensor.
 *
 * @return The humidity reading as a percentage.
 */
float readHumidity() {
  return sht.readHumidity();
}
